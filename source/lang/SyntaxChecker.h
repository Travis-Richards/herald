#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

namespace parse_tree {

class Visitor;

} // namespace parse_tree

/// Enumerates the several possible
/// syntax errors.
enum class SyntaxErrorID : int {
  /// A place holder for a "null" syntax error.
  None,
  /// The case of an integer token being a type other than a number.
  InvalidIntegerValue,
  /// An invalid symbol used for an integer sign.
  InvalidSignSymbol,
  /// A size vlaue that was negative.
  InvalidSizeValue,
  /// The case of an integer missing a value token.
  MissingIntegerValue,
  /// An integer with missing matrix values.
  MissingMatrixIntegers
};

/// Represents an arbitrary syntax error.
class SyntaxError {
  /// The ID of the syntax error.
  SyntaxErrorID id;
public:
  /// Constructs a base instance of a syntax error.
  constexpr SyntaxError(SyntaxErrorID id_) noexcept : id(id_) { }
  /// Just a stub.
  virtual ~SyntaxError() {}
  /// Accesses the description of the syntax error.
  /// @returns A null-terminated string describing the error.
  virtual const char* get_description() const noexcept = 0;
  /// Accesses the ID of the syntax error.
  inline SyntaxErrorID get_id() const noexcept {
    return id;
  }
};

/// The base of the syntax error list.
class SyntaxErrorList {
public:
  /// Creates a new syntax error list.
  /// @returns A new syntax error list instance.
  static ScopedPtr<SyntaxErrorList> make();
  /// Just a stub.
  virtual ~SyntaxErrorList() {}
  /// Adds a syntax error to the list.
  virtual void add(SyntaxError* error) = 0;
  /// Accesses a syntax error at a specific index.
  /// @param index The index of the error to get.
  /// @returns A pointer to the syntax error on success.
  /// If the index is out of bounds, then a null pointer is returned.
  virtual const SyntaxError* at(std::size_t index) const noexcept = 0;
  /// Indicates the number of entries in the error list.
  /// A size of zero indicates no errors were found.
  virtual std::size_t size() const noexcept = 0;
};

/// Creates a syntax checker.
/// @param list A list to store syntax errors into.
/// @returns A new instance of a syntax checker as a visitor interface.
ScopedPtr<parse_tree::Visitor> make_syntax_checker(SyntaxErrorList* list);

} // namespace herald
