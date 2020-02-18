#include <herald/protocol/ParseTree.h>

#include <herald/ScopedPtr.h>

#include <herald/protocol/Token.h>

#include <vector>

namespace herald {

namespace protocol {

namespace {

/// The full matrix implementation.
class MatrixImpl final : public Matrix {
  /// The integers making up the matrix.
  std::vector<Integer> values;
public:
  /// Constructs a new instance of the matrix implementation.
  MatrixImpl(const Size& s) noexcept : Matrix(s) {}
  /// Adds an integer to the matrix.
  /// @param v The value to add.
  void add(const Integer& v) override {
    values.emplace_back(v);
  }
  /// Accesses an integer from the matrix.
  /// @param index The index of the number to access.
  /// @returns The specified integer value.
  Integer get_integer(std::size_t index) const noexcept override {
    if (index >= values.size()) {
      return Integer(nullptr, nullptr);
    } else {
      return values[index];
    }
  }
  /// Accesses the number of integers in the matrix.
  std::size_t get_integer_count() const noexcept override {
    return values.size();
  }
};

} // namespace

bool Integer::valid() const noexcept {
  return value && value->has_type(TokenType::Number);
}

bool Integer::is_negative() const noexcept {
  return sign && sign->has_type(TokenType::NegativeSign);
}

bool Integer::to_signed_value(int& n) const noexcept {

  unsigned int n_unsigned = 0;

  if (!to_unsigned_value(n_unsigned)) {
    return false;
  } else {
    n = (int) n_unsigned;
    n *= is_negative() ? -1 : 1;
    return true;
  }
}

bool Integer::to_unsigned_value(unsigned int& n) const noexcept {

  if (!value || !value->has_type(TokenType::Number)) {
    return false;
  }

  n = 0;

  for (std::size_t i = 0; i < value->get_size(); i++) {

    auto c = value->at(i);
    if ((c < '0')
     || (c > '9')) {
      return false;
    }

    n *= 10;
    n += int(c - '0');
  }

  return true;
}

bool Size::to_values(int& w, int& h) const noexcept {
  return width.to_signed_value(w)
     && height.to_signed_value(h)
     && (w > 0) && (h > 0);
}

ScopedPtr<Matrix> Matrix::make(const Size& s) {
  return new MatrixImpl(s);
}

} // namespace protocol

} // namespace herald
