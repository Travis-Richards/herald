#include <herald/protocol/Parser.h>

#include <herald/protocol/ParseTree.h>

#include <herald/ScopedPtr.h>

#include "Token.h"

namespace herald {

namespace protocol {

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
  Integer parse_integer() noexcept override;
  /// Parses for a size structure.
  Size parse_size() noexcept override;
  /// Parses for a matrix.
  ScopedPtr<Matrix> parse_matrix() override;
  /// Parses an arbitrary node.
  ScopedPtr<Node> parse_any() override;
protected:
  /// Parses a "set_action" statement.
  ScopedPtr<SetActionStmt> parse_set_action_stmt() override;
  /// Checks if the next token is a specific identifer.
  /// If it is, the parser will move passed it.
  /// @param id The identifier to check for.
  /// @returns True on a match, false otherwise.
  bool match_identifier(const char* id) {

    const auto* id_token = get_token(0);
    if (!id_token) {
      return false;
    } else if (!id_token->has_type(TokenType::Identifier)) {
      return false;
    } else if (!id_token->has_data(id)) {
      return false;
    }

    next(1);

    return true;
  }
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

Integer ParserImpl::parse_integer() noexcept {

  const auto* first = get_token(0);

  const auto* sign = (first && first->has_type(TokenType::NegativeSign)) ? first : nullptr;

  std::size_t value_offset = sign ? 1 : 0;

  const auto* value = get_token(value_offset);

  next(value_offset + 1);

  return Integer(sign, value);
}

Size ParserImpl::parse_size() noexcept {
  auto w = parse_integer();
  auto h = parse_integer();
  return Size(w, h);
}

ScopedPtr<Matrix> ParserImpl::parse_matrix() {

  auto size = parse_size();

  auto matrix = Matrix::make(size);

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

ScopedPtr<Node> ParserImpl::parse_any() {

  auto set_action_stmt = parse_set_action_stmt();
  if (set_action_stmt) {
    return set_action_stmt;
  }

  return nullptr;
}

ScopedPtr<SetActionStmt> ParserImpl::parse_set_action_stmt() {

  if (!match_identifier("set_action")) {
    return nullptr;
  }

  auto object = parse_integer();
  auto action = parse_integer();
  return new SetActionStmt(object, action);
}

} // namespace

ScopedPtr<Parser> Parser::make(const Token* tokens, std::size_t count) {
  return new ParserImpl(tokens, count);
}

} // namespace protocol

} // namespace herald
