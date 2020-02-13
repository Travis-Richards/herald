#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

class Token;

namespace parse_tree {

class Integer;
class Size;
class Matrix;

/// The base for any class that
/// is used to interpret the parse tree.
class Visitor {
public:
  /// Just a stub.
  virtual ~Visitor() {}
  /// Visits an integer node.
  virtual void visit(const Integer&) = 0;
  /// Visits a matrix.
  virtual void visit(const Matrix&) = 0;
  /// Visits a size node.
  virtual void visit(const Size&) = 0;
};

/// The base of any structure
/// based from syntax.
class Node {
public:
  /// Just a stub.
  virtual ~Node() {}
  /// Accepts a node visitor.
  /// @param visitor The visitor to accept.
  virtual void accept(Visitor& visitor) const = 0;
};

/// An integer value.
/// May be signed but will not
/// contain a decimal point or
/// fractional values.
class Integer final : public Node {
  /// The sign of the integer.
  /// This token may be null,
  /// in which case the integer
  /// can be considered to be positive.
  const Token* sign;
  /// The value of the integer.
  /// A null token here should
  /// be considered an error.
  const Token* value;
public:
  /// Constructs an integer node.
  /// @param s The sign token to assign.
  /// This may be a null pointer.
  /// @param v The value token of the integer.
  /// This must not be null.
  constexpr Integer(const Token* s,
                    const Token* v) noexcept
    : sign(s), value(v) {}
  /// Accepts a visitor.
  void accept(Visitor& visitor) const override {
    visitor.visit(*this);
  }
  /// Accesses the sign token.
  const Token* get_sign_token() const noexcept {
    return sign;
  }
  /// Accesses the value token.
  const Token* get_value_token() const noexcept {
    return value;
  }
  /// Indicates whether or not the integer is negative.
  bool is_negative() const noexcept;
  /// Converts the integer to a signed integer value.
  /// @returns True on success, false on failure.
  bool to_signed_value(int& value) const noexcept;
  /// Converts the integer to an unsigned value.
  /// The sign token is ignored in this function.
  /// @returns True on success, false on failure.
  bool to_unsigned_value(unsigned int& value) const noexcept;
  /// Indicates whether or not the integer is valid.
  bool valid() const noexcept;
};

/// Specifies width and height.
/// May be used to specified the
/// size of a matrix.
class Size final : public Node {
  /// The width specification.
  Integer width;
  /// The height specification.
  Integer height;
public:
  /// Constructs a size specification.
  /// @param w The width to assign.
  /// @param h The height to assign.
  constexpr Size(const Integer& w,
                 const Integer& h) noexcept
    : width(w), height(h) {}
  /// Accepts a visitor.
  void accept(Visitor& visitor) const override {
    visitor.visit(*this);
  }
  /// Accesses the width of the integer.
  inline const Integer& get_width() const noexcept {
    return width;
  }
  /// Accesses the height integer.
  inline const Integer& get_height() const noexcept {
    return height;
  }
  /// Converts the width and height nodes to integers.
  /// @returns True on success, false on failure.
  bool to_values(int &w, int& h) const noexcept;
  /// Quickly determines wether
  /// or not the size is valid.
  inline bool valid() const noexcept {
    return width.valid() && !width.is_negative()
       && height.valid() && !height.is_negative();
  }
};

/// Represents an integer matrix.
class Matrix : public Node {
  /// The size of the matrix.
  Size size;
public:
  /// Creates a new matrix instance.
  /// @param s The size of the matrix.
  /// @returns A new matrix instance.
  static ScopedPtr<Matrix> make(const Size& s);
  /// Constructs the base matrix class.
  constexpr Matrix(const Size& s) noexcept : size(s) {}
  /// Just a stub.
  virtual ~Matrix() {}
  /// Accepts a visitor.
  void accept(Visitor& visitor) const override {
    visitor.visit(*this);
  }
  /// Adds an integer to the matrix.
  /// @param i The integer to add.
  virtual void add(const Integer& i) = 0;
  /// Accesses the size of the matrix.
  /// @returns The size specification of the matrix.
  inline Size get_size() const noexcept {
    return size;
  }
  /// Accesses an integer from the matrix.
  /// @param index The index of the integer to get.
  /// @returns The integer at the specified index.
  virtual Integer get_integer(std::size_t index) const noexcept = 0;
  /// Accesses the number of integers in the marix.
  virtual std::size_t get_integer_count() const noexcept = 0;
};

} // namespace parse_tree

} // namespace herald
