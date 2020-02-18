#include <herald/protocol/SyntaxChecker.h>

#include <herald/protocol/ParseTree.h>

#include <herald/ScopedPtr.h>

#include "Token.h"

#include <sstream>
#include <vector>

namespace herald {

namespace protocol {

namespace {

/// An implementation of the syntax error interface.
class SyntaxErrorImpl final : public SyntaxError {
  /// A description of the syntax error.
  std::string description;
public:
  /// Constructs an instance of the syntax error.
  /// @param id The ID of the syntax error.
  /// @param d A description of the error that occurred.
  SyntaxErrorImpl(SyntaxErrorID id, std::string&& d)
    : SyntaxError(id), description(std::move(d)) {}
  /// Accesses a decsription of the syntax error.
  const char* get_description() const noexcept override {
    return description.c_str();
  }
};

/// Used for verifying the correctness
/// of a syntax node.
class SyntaxChecker final : public Visitor {
  /// The list of syntax errors to add to.
  SyntaxErrorList* errors;
public:
  /// Constructs a new syntax checker.
  /// @param e The error list to add to.
  SyntaxChecker(SyntaxErrorList* e) : errors(e) {}
  /// Checks an integer instance.
  void visit(const Integer& integer) override {

    const auto* sign = integer.get_sign_token();

    if (sign && !sign->has_type(TokenType::NegativeSign)) {
      auto formatter = [sign](std::ostream& err) {
        err << "Integer sign symbol " << *sign << " invalid.";
      };
      format_error(SyntaxErrorID::InvalidSignSymbol, formatter);
    }

    const auto* value = integer.get_value_token();

    if (!value) {
      auto formatter = [](std::ostream& err) {
        err << "Missing integer value.";
      };
      format_error(SyntaxErrorID::MissingIntegerValue, formatter);
    } else if (!value->has_type(TokenType::Number)) {
      auto formatter = [value](std::ostream& err) {
        err << "Invalid integer value " << *value << ".";
      };
      format_error(SyntaxErrorID::InvalidIntegerValue, formatter);
    }
  }
  /// Checks the "set action" statement.
  void visit(const SetActionStmt& stmt) override {
    // TODO
    (void)stmt;
  }
  /// Checks a size instance.
  void visit(const Size& size) override {
    check_size_integer("width", size.get_width());
    check_size_integer("height", size.get_height());
  }
  /// Checks a matrix instance.
  void visit(const Matrix& matrix) override {

    auto size = matrix.get_size();

    visit(size);

    int w = 0;
    int h = 0;

    size.get_width().to_signed_value(w);
    size.get_height().to_signed_value(h);

    if ((w >= 0) && (h >= 0) && ((std::size_t)(w * h) != matrix.get_integer_count())) {

      auto formatter = [w, h, &matrix](std::ostream& err) {
        err << "Expected " << w << "x" << h;
        err << " matrix to have " << w * h << " values,";
        err << " but only " << matrix.get_integer_count() << " were found.";
      };

      format_error(SyntaxErrorID::MissingMatrixIntegers, formatter);
    }

    auto count = matrix.get_integer_count();
    for (decltype(count) i = 0; i < count; i++) {
      visit(matrix.get_integer(i));
    }
  }
protected:
  /// Checks an integer used to specify a size.
  /// @param name The name of the size field.
  /// @param integer The integer containing the size value.
  void check_size_integer(const char* name, const Integer& integer) {
    if (integer.is_negative()) {
      auto formatter = [name](std::ostream& err) {
        err << "Size '" << name << "' specifier must not be negative.";
      };
      format_error(SyntaxErrorID::InvalidSizeValue, formatter);
    }
  }
  /// Formats an error message and adds it to the error list.
  /// @tparam Formatter The formatting function type.
  /// @param formatter A functor used to format the error message.
  template <typename Formatter>
  void format_error(SyntaxErrorID id, Formatter formatter) {
    std::ostringstream stream;
    formatter(stream);
    errors->add(new SyntaxErrorImpl(id, stream.str()));
  }
};

/// An implementation of the syntax error list interface.
class SyntaxErrorListImpl final : public SyntaxErrorList {
  /// The syntax errors added to the list.
  std::vector<ScopedPtr<SyntaxError>> errors;
public:
  /// Adds an error to the list.
  void add(SyntaxError* error) override {
    errors.emplace_back(error);
  }
  /// Accesses an error at a specific index.
  const SyntaxError* at(std::size_t index) const noexcept override {
    return (index < errors.size()) ? errors[index].get() : nullptr;
  }
  /// Indicates the size of the list.
  std::size_t size() const noexcept override {
    return errors.size();
  }
};

} // namespace

ScopedPtr<SyntaxErrorList> SyntaxErrorList::make() {
  return new SyntaxErrorListImpl;
}

ScopedPtr<Visitor> make_syntax_checker(SyntaxErrorList* errors) {
  return new SyntaxChecker(errors);
}

} // namespace protocol

} // namespace herald
