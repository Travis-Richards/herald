#pragma once

#include <cstddef>

/// Enumerates the several types of tokens
/// that can be found by the lexer.
enum class TokenType : int {
  /// An identifier sequence.
  /// This includes: a-z, A-Z, '_'
  Identifier,
  /// Used when read error occurs
  /// or when an invalid token is found.
  Invalid,
  /// The '-' character.
  NegativeSign,
  /// A newline sequence.
  Newline,
  /// A number token.
  Number,
  /// A space or tab sequence.
  /// This is usually ignored.
  Space,
  /// Either a single or double
  /// quoted string.
  StringLiteral,
  /// A single or double quoted
  /// string without a terminated
  /// quotation symbol.
  UnterminatedStringLiteral
};

/// A single unit of text produced by
/// the lexer and usable by the parser.
class Token final {
  /// The type of this token.
  TokenType type;
  /// The character data that was found.
  const char* data;
  /// The number of characters in the token.
  std::size_t size;
  /// The offset of the token along the
  /// original input data. This may be
  /// helpful in displaying an error message.
  std::size_t offset;
public:
  /// Constructs a new token instance.
  /// @param t The type of this token.
  /// @param d The character data of the token.
  /// @param s The number of characters in the token.
  /// @param o The offset of the token along the
  /// origin input data.
  constexpr Token(TokenType t,
                  const char* d,
                  std::size_t s,
                  std::size_t o) noexcept
    : type(t),
      data(d),
      size(s),
      offset(o) {}
  /// Creates an invalid token instance.
  /// @returns An invalid token instance.
  static constexpr Token invalid() noexcept {
    return Token(TokenType::Invalid, "", 0, 0);
  }
  /// Accesses a token character at a specified index.
  inline char at(std::size_t index) const noexcept {
    return (index < size) ? data[index] : 0;
  }
  /// Accesses the number of characters in the token.
  inline std::size_t get_size() const noexcept {
    return size;
  }
  /// Indicates if the token has a certain type.
  /// @param t The type to check for.
  /// @returns True if the types are equal, false otherwise.
  inline bool has_type(TokenType t) const noexcept {
    return type == t;
  }
};
