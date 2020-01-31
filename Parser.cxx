#include "Parser.h"

#include "Lexer.h"

#include <QLatin1String>

#include <iostream>

namespace {

/// This structure is used for
/// containing simple token data.
struct Token final {
  /// The type of the token.
  TokenType type;
  /// The token's character data.
  QStringRef data;
  /// Constructs a new token instance.
  Token(TokenType t, const QStringRef& d)
    : type(t), data(d) {}
  /// Constructs an invalid token.
  Token() : type(TokenType::Invalid) {}
  /// Indicates if the token
  /// has specified string content.
  bool equal_to(const QLatin1String& str) const {
    return data.compare(str) == 0;
  }
};

/// Converts a string reference to a string.
/// @param ref The reference to convert.
QString to_string(const QStringRef& ref) {
  QString out;
  out.append(ref);
  return out;
}

} // namespace

/// A class for containing a token list.
class TokenList : public QObject {
  /// The tokens of the token list.
  QList<Token> tokens;
  /// Whether or not the list is line terminated.
  bool line_terminated;
  /// A token returned in out of bounds conditions.
  Token null_token;
public:
  /// Constructs a new token list.
  /// @param parent A pointer to the parent object.
  /// The only reasonable choice for this parameter
  /// is a pointer to the parser instance.
  TokenList(QObject* parent) : QObject(parent), line_terminated(false) {}
  /// Adds a new token to the list.
  void add(TokenType type, const QStringRef& data) {
    tokens.push_back(Token(type, data));
    line_terminated = (type == TokenType::Newline);
  }
  /// Gets a token at a certain index.
  const Token& at(int index) const noexcept {
    if ((index > tokens.size()) || (index < 0)) {
      return null_token;
    } else {
      return tokens[index];
    }
  }
  /// Checks if a token at a certain index
  /// has specified token type.
  bool check_eq(int index, TokenType type) const {
    return at(index).type == type;
  }
  /// Checks if a token at a certain index
  /// has specified string content.
  bool check_eq(int index, const char* str) const {
    return at(index).equal_to(QLatin1String(str));
  }
  /// Indicates if the tokne list is empty or not.
  bool empty() const {
    return tokens.empty();
  }
  /// Filters out unused tokens.
  void filter() {
    int i = 0;
    while (i < tokens.size()) {
      if ((tokens[i].type == TokenType::Space)
       || (tokens[i].type == TokenType::Newline)) {
        tokens.removeAt(i);
      } else {
        i++;
      }
    }
  }
  /// Indicates if the token list is line terminated.
  bool is_line_terminated() const noexcept {
    return line_terminated;
  }
  /// Resets the token list to its
  /// initial conditions.
  void reset() {
    tokens.clear();
    line_terminated = false;
  }
  /// Indicates the number of tokens in the list.
  int size() const {
    return tokens.size();
  }
};

Parser::Parser(const QString& input, QObject* parent) : QObject(parent) {
  tokens = new TokenList(this);
  lexer = new Lexer(input, this);
  connect(lexer, &Lexer::token_found, this, &Parser::on_token);
}

bool Parser::parse() {

  tokens->reset();

  while (!tokens->is_line_terminated()) {
    if (!lexer->scan()) {
      break;
    }
  }

  tokens->filter();

  if (tokens->empty()) {
    return false;
  }

  return parse_background_stmt()
      || parse_finish_stmt()
      || parse_image_texture_stmt();
}

bool Parser::parse_background_stmt() {

  if (tokens->size() != 2) {
    return false;
  }

  if (!tokens->check_eq(0, "set_background")) {
    return false;
  }

  if (!tokens->check_eq(1, TokenType::Number)) {
    return false;
  }

  bool ok = false;

  int texture_id = tokens->at(1).data.toInt(&ok);

  if (ok) {
    emit found_background(texture_id);
    return true;
  } else {
    // TODO : error message
    return false;
  }
}

bool Parser::parse_finish_stmt() {

  if (tokens->size() != 1) {
    return false;
  }

  if (!tokens->check_eq(0, "finish")) {
    return false;
  }

  emit found_finish();

  return true;
}

bool Parser::parse_image_texture_stmt() {

  if (tokens->size() != 2) {
    return false;
  }

  if (!tokens->check_eq(0, "load_image_texture")) {
    return false;
  }

  if (!tokens->check_eq(1, TokenType::StringLiteral)) {
    return false;
  }

  emit found_image_texture(to_string(tokens->at(1).data));

  return true;
}

void Parser::on_token(TokenType type, const QStringRef& data) {
  tokens->add(type, data);
}
