#pragma once

#include <cstddef>

namespace herald {

template <typename T>
class ScopedPtr;

namespace protocol {

class Integer;
class Matrix;
class Node;
class SetActionStmt;
class Size;
class Token;

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
  /// Parses an arbitrary node.
  /// @returns A pointer to the node that was found.
  virtual ScopedPtr<Node> parse_any() = 0;
  /// Parses an integer.
  /// @returns An integer node.
  /// Must be validated before using.
  virtual Integer parse_integer() noexcept = 0;
  /// Parses a "set_action" statement.
  /// @returns On success, a pointer to a "set_action" statement.
  /// On failure, a null pointer.
  virtual ScopedPtr<SetActionStmt> parse_set_action_stmt() = 0;
  /// Parses for a size specifier.
  /// @returns A size node.
  /// Must be validated before using.
  virtual Size parse_size() noexcept = 0;
  /// Parses for a matrix.
  virtual ScopedPtr<Matrix> parse_matrix() = 0;
};

} // namespace protocol

} // namespace herald
