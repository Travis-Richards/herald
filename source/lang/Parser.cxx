#include "Parser.h"

#include "Lexer.h"
#include "Matrix.h"
#include "ParseTree.h"
#include "ScopedPtr.h"

#include <QSize>

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
    if ((index >= tokens.size()) || (index < 0)) {
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
  position = 0;
  connect(lexer, &Lexer::token_found, this, &Parser::on_token);
}

ScopedPtr<BuildRoomResponse> Parser::parse_build_room_response() {

  if (!prepare_tokens()) {
    return nullptr;
  }

  QSize room_size;

  if (!parse_size(room_size)) {
    return nullptr;
  }

  auto texture_matrix = Matrix::make(room_size);

  if (!parse_matrix(*texture_matrix)) {
    return nullptr;
  }

  auto frame_matrix = Matrix::make(room_size);

  if (!parse_matrix(*frame_matrix)) {
    return nullptr;
  }

  auto flag_matrix = Matrix::make(room_size);

  if (!parse_matrix(*flag_matrix)) {
    return nullptr;
  }

  return BuildRoomResponse::make(texture_matrix.release(),
                                 frame_matrix.release(),
                                 flag_matrix.release());
}

ScopedPtr<FillObjectsResponse> Parser::parse_fill_objects_response() {

  if (!prepare_tokens()) {
    return nullptr;
  }

  QSize room_size;

  if (!parse_size(room_size)) {
    return nullptr;
  }

  auto action_matrix = Matrix::make(room_size);

  if (!parse_matrix(*action_matrix)) {
    return nullptr;
  }

  auto flag_matrix = Matrix::make(room_size);

  if (!parse_matrix(*flag_matrix)) {
    return nullptr;
  }

  return new FillObjectsResponse(action_matrix.release(),
                                 flag_matrix.release());
}

ScopedPtr<SetBackgroundResponse> Parser::parse_set_background_response() {

  if (!prepare_tokens()) {
    return nullptr;
  }

  int animation = 0;

  if (!parse_int(animation)) {
    return nullptr;
  }

  int frame = 0;

  if (!parse_int(frame)) {
    return new SetBackgroundResponse(animation, 0);
  }

  return new SetBackgroundResponse(animation, frame);
}

bool Parser::fail(const QString& msg) {
  emit error(msg);
  return false;
}

bool Parser::prepare_tokens() {

  position = 0;

  tokens->reset();

  while (!tokens->is_line_terminated()) {
    if (!lexer->scan()) {
      break;
    }
  }

  tokens->filter();

  return !tokens->empty();
}

bool Parser::parse_matrix(Matrix& matrix) {

  for (int y = 0; y < matrix.height(); y++) {
    for (int x = 0; x < matrix.width(); x++) {
      int value = 0;
      if (!parse_int(value)) {
        return fail(tr("Missing matrix value"));
      } else {
        matrix.set(x, y, value);
      }
    }
  }

  return true;
}

bool Parser::parse_size(QSize& size) {
  int w = 0;
  int h = 0;
  if (!parse_int(w)) {
    return fail(tr("Missing width specifier"));
  } else if (!parse_int(h)) {
    return fail(tr("Missing height specifier"));
  } else {
    size = QSize(w, h);
    return true;
  }
}

bool Parser::parse_int(int& value) {

  if (!tokens->check_eq(position, TokenType::Number)) {
    return false;
  }

  bool ok = false;

  value = tokens->at(position).data.toInt(&ok);

  position++;

  return ok;
}

void Parser::on_token(TokenType type, const QStringRef& data) {
  tokens->add(type, data);
}
