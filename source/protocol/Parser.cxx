#include "Parser.h"

#include "ParseTree.h"
#include "ScopedPtr.h"
#include "Token.h"

namespace herald {

namespace {

/// Implements the parser interface.
class ParserImpl final : public Parser {
  /// The array of tokens being parsed.
  const Token* tokens;
  /// The number of tokens in the token array.
  std::size_t count;
  /// The position of the parser within the token array.
  std::size_t pos;
public:
  /// Constructs an instance of the parser implementation.
  /// @param t The tokens to be parsed.
  /// @param c The number of tokens in the token array.
  constexpr ParserImpl(const Token* t, std::size_t c) noexcept
    : tokens(t), count(c), pos(0) {}
  /// Indicates whether or not the parser
  /// has reached the end of the input.
  bool done() const noexcept override {
    return pos >= count;
  }
  /// Parses for an integer.
  /// @returns A new integer instance.
  parse_tree::Integer parse_integer() noexcept override;
  /// Parses for a size structure.
  parse_tree::Size parse_size() noexcept override;
  /// Parses for a matrix.
  ScopedPtr<parse_tree::Matrix> parse_matrix() override;
protected:
  /// Safely indicates if a token has a certain type.
  /// @param token A pointer of the token to check for.
  /// @param type The type to check for.
  /// @returns If the token has the specified type, true is returned.
  /// If either the token doesn't have the type or the token
  /// pointer is null, false is returned instead.
  static inline bool has_type(const Token* token, TokenType type) noexcept {
    return token ? token->has_type(type) : false;
  }
  /// Accesses a token relative to the parser's current position.
  /// @param offset The offset from the current position to
  /// get the token of.
  /// @returns The token at the specified offset.
  /// If the offset is out of bounds, then a null
  /// pointer is returned instead.
  const Token* get_token(std::size_t offset) const noexcept {
    if (out_of_bounds(offset)) {
      return nullptr;
    } else {
      return &tokens[pos + offset];
    }
  }
  /// Goes forward a certain number of tokens.
  void next(std::size_t n) noexcept {
    while (!done() && (n > 0)) {
      next_significant_token();
      n--;
    }
  }
  /// Goes to the next non-whitespace token.
  void next_significant_token() noexcept {
    pos++;
    while (pos < count) {
      if (!tokens[pos].has_type(TokenType::Space)
       && !tokens[pos].has_type(TokenType::Newline)) {
        break;
      } else {
        pos++;
      }
    }
  }
  /// Indicates if a certain token offset
  /// is out of bounds.
  bool out_of_bounds(std::size_t offset) const noexcept {
    return (pos + offset) >= count;
  }
};

parse_tree::Integer ParserImpl::parse_integer() noexcept {

  const auto* first = get_token(0);

  const auto* sign = (first && first->has_type(TokenType::NegativeSign)) ? first : nullptr;

  std::size_t value_offset = sign ? 1 : 0;

  const auto* value = get_token(value_offset);

  next(value_offset + 1);

  return parse_tree::Integer(sign, value);
}

parse_tree::Size ParserImpl::parse_size() noexcept {
  auto w = parse_integer();
  auto h = parse_integer();
  return parse_tree::Size(w, h);
}

ScopedPtr<parse_tree::Matrix> ParserImpl::parse_matrix() {

  auto size = parse_size();

  auto matrix = parse_tree::Matrix::make(size);

  if (!size.valid()) {
    return matrix;
  }

  int w = 0;
  int h = 0;

  if (!size.to_values(w, h)) {
    return matrix;
  }

  for (int i = 0; (i < (w * h)) && !done(); i++) {
    auto value = parse_integer();
    if (!value.valid()) {
      break;
    } else {
      matrix->add(value);
    }
  }

  return matrix;
}

} // namespace

ScopedPtr<Parser> Parser::make(const Token* tokens, std::size_t count) {
  return new ParserImpl(tokens, count);
}

} // namespace herald
