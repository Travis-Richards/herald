#include "Lexer.h"

#include <herald/ScopedPtr.h>

#include "Token.h"

namespace herald {

namespace {

/// An implementation of the lexer interface.
class LexerImpl final : public Lexer {
  /// The characters to be scanned.
  const char* data;
  /// The number of characters in the data string.
  std::size_t size;
  /// The position of the lexer among the data.
  std::size_t pos;
public:
  /// Constructs a new instance of the lexer implementation.
  /// @param d The data string to scan.
  /// @param s The number of characters in the data string.
  constexpr LexerImpl(const char* d, std::size_t s) noexcept
    : data(d), size(s), pos(0) {}
  /// Indicates whether or not the lexer
  /// has reached the end of its input.
  bool done() const noexcept override {
    return pos >= size;
  }
  /// Scans for a token.
  /// @returns The token that was found.
  /// If the end of the input was reached,
  /// then an invalid token instance is
  /// returned instead.
  Token scan() noexcept override;
protected:
  /// Indicates whether or not an offset
  /// is out of bounds.
  /// @param offset The offset to check boundaries for.
  /// @returns True if the offset is out of bounds,
  /// false if it is not.
  bool out_of_bounds(std::size_t offset) const noexcept {
    return (pos + offset) >= size;
  }
  /// Gets the number of characters remaining to be scanned.
  std::size_t remaining() const noexcept {
    return (pos < size) ? (size - pos) : 0;
  }
  /// Gets a character from the input
  /// at a certain offset.
  char peek(std::size_t offset) const noexcept {
    return out_of_bounds(offset) ? 0 : data[pos + offset];
  }
  /// Goes to the next series of characters
  /// from the current position.
  void next(std::size_t count) noexcept {
    pos += count;
  }
  /// Completes a newline token.
  Token complete_newline() noexcept;
  /// Completes a number token.
  Token complete_number() noexcept;
  /// Completes an identifier token.
  Token complete_identifier() noexcept;
  /// Completes a space token.
  Token complete_space() noexcept;
  /// Completes a string literal token.
  /// @param quote The quote character
  /// that begins the string literal.
  Token complete_string_literal(const char quote) noexcept;
  /// Completes a token and emits the signal.
  Token complete(TokenType type, int size) noexcept;
  /// Indicates whether or not a character
  /// is a decimal digit.
  static bool is_digit(char c) noexcept {
    return ((c >= '0') && (c <= '9'));
  }
  /// Indicates whether or not a character
  /// could be part of an identifier body.
  static bool is_identifier(char c) noexcept {
    return is_letter(c) || is_digit(c) || (c == '_');
  }
  /// Indicates whether or not a character
  /// is a letter.
  static bool is_letter(char c) noexcept {
    return ((c >= 'a') && (c <= 'z'))
        || ((c >= 'A') && (c <= 'z'));
  }
};

Token LexerImpl::scan() noexcept {

  if (done()) {
    return Token::invalid();
  }

  auto first = peek(0);
  if (first == '\r') {
    return complete_newline();
  } else if (first == '\n') {
    return complete(TokenType::Newline, 1);
  } else if ((first == ' ') || (first == '\t')) {
    return complete_space();
  } else if ((first == '\'') || (first == '"')) {
    return complete_string_literal(first);
  } else if (is_digit(peek(0))) {
    return complete_number();
  } else if (first == '-') {
    return complete(TokenType::NegativeSign, 1);
  }

  if (is_letter(first) || (first == '_')) {
    return complete_identifier();
  }

  if (is_digit(first)) {
    return complete_number();
  }

  return complete(TokenType::Invalid, 1);
}

Token LexerImpl::complete_newline() noexcept {
  auto line_feed = peek(1);
  if (line_feed == '\n') {
    return complete(TokenType::Newline, 2);
  } else {
    return complete(TokenType::Newline, 1);
  }
}

Token LexerImpl::complete_number() noexcept {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto ch = peek(i);
    if (!is_digit(ch)
     && (ch != '.')
     && (ch != '-')
     && (ch != 'e')) {
      return complete(TokenType::Number, i);
    }
  }

  return complete(TokenType::Number, remaining());
}

Token LexerImpl::complete_identifier() noexcept {

  for (int i = 1; !out_of_bounds(i); i++) {
    if (!is_identifier(peek(i))) {
      return complete(TokenType::Identifier, i);
    }
  }

  return complete(TokenType::Identifier, remaining());
}

Token LexerImpl::complete_space() noexcept {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto ch = peek(i);
    if ((ch != ' ') && (ch != '\t')) {
      return complete(TokenType::Space, i);
    }
  }

  return complete(TokenType::Space, remaining());
}

Token LexerImpl::complete_string_literal(char quote) noexcept {

  for (int i = 1; !out_of_bounds(i); i++) {
    auto c = peek(i);
    if (c == quote) {
      Token token(TokenType::StringLiteral,
                  data + pos + 1,
                  i - 1, i);
      next(i + 1);
      return token;
    } else if (c == '\\') {
      i++;
    }
  }

  return complete(TokenType::UnterminatedStringLiteral, remaining());
}

Token LexerImpl::complete(TokenType type, int size) noexcept {
  Token token(type, data + pos, size, pos);
  next(size);
  return token;
}

} // namespace

ScopedPtr<Lexer> Lexer::make(const char* data, std::size_t size) {
  return new LexerImpl(data, size);
}

} // namespace herald
