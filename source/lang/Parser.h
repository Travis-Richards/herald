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

} // namespace parse_tree

/// Used for parsing responses from the game.
class Parser {
public:
  /// Creates a new parser instance.
  /// @param tokens The tokens to be parsed.
  /// @param count The number of tokens to parse.
  static ScopedPtr<Parser> make(const Token* tokens, std::size_t count);
  /// Just a stub.
  virtual ~Parser() {}
  /// Indicates when the parser has reached the
  /// end of the token array.
  virtual bool done() const noexcept = 0;
  /// Parses an integer.
  /// @returns An integer node.
  /// Must be validated before using.
  virtual parse_tree::Integer parse_integer() noexcept = 0;
  /// Parses for a size specifier.
  /// @returns A size node.
  /// Must be validated before using.
  virtual parse_tree::Size parse_size() noexcept = 0;
  /// Parses for a matrix.
  virtual ScopedPtr<parse_tree::Matrix> parse_matrix() = 0;
};

} // namespace herald
