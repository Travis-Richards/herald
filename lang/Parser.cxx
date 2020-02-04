#include "Parser.h"

#include "Lexer.h"
#include "ParseTree.h"

#include <QColor>
#include <QLatin1String>
#include <QPolygonF>

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

class ColorTextureDeclImpl final : public ColorTextureDecl {
  QColor color;
public:
  ColorTextureDeclImpl(const QColor& c) : color(c) { }

  const QColor& get_color() const noexcept override {
    return color;
  }
};

class ImageTextureDeclImpl final : public ImageTextureDecl {
  QString path;
public:
  ImageTextureDeclImpl(const QStringRef& path_ref) {
    path.append(path_ref);
  }
  const QString& get_image_path() const noexcept override {
    return path;
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
      || parse_color_texture_decl()
      || parse_draw_box_stmt()
      || parse_finish_stmt()
      || parse_image_texture_decl();
}

bool Parser::parse_background_stmt() {

  if (tokens->size() != 2) {
    return false;
  }

  if (!tokens->check_eq(0, "set_background")) {
    return false;
  }

  int texture_id = 0;

  if (!parse_int(1, texture_id)) {
    return false;
  }

  emit found_node(BackgroundStmt(texture_id));

  return true;
}

bool Parser::parse_color_texture_decl() {

  if (tokens->size() != 5) {
    return false;
  }

  if (!tokens->check_eq(0, "decl_color_texture")) {
    return false;
  }

  QColor color;

  if (!parse_color(1, color)) {
    return false;
  }

  ColorTextureDeclImpl color_texture_decl(color);

  emit found_node(color_texture_decl);

  return true;
}

bool Parser::parse_draw_box_stmt() {

  if (tokens->size() < 6) {
    return false;
  }

  if (!tokens->check_eq(0, "draw_box")) {
    return false;
  }

  int texture_id = 0;

  if (!parse_int(1, texture_id)) {
    return false;
  }

  QPointF points[2];

  if (!parse_point(2, points[0])
   || !parse_point(4, points[1])) {
    return false;
  }

  DrawBoxStmt draw_box_stmt(points[0], points[1], texture_id);

  emit found_node(draw_box_stmt);

  return true;
}

bool Parser::parse_finish_stmt() {

  if (tokens->size() != 1) {
    return false;
  }

  if (!tokens->check_eq(0, "finish")) {
    return false;
  }

  emit found_node(FinishStmt());

  return true;
}

bool Parser::parse_image_texture_decl() {

  if (tokens->size() != 2) {
    return false;
  }

  if (!tokens->check_eq(0, "load_image_texture")) {
    return false;
  }

  if (!tokens->check_eq(1, TokenType::StringLiteral)) {
    return false;
  }

  ImageTextureDeclImpl image_texture(tokens->at(1).data);

  emit found_node(image_texture);

  return true;
}

int Parser::parse_rect(int offset, QRectF& rect) {

  QPointF points[4];

  if (!parse_point(offset + 0, points[0])
   || !parse_point(offset + 2, points[1])
   || !parse_point(offset + 4, points[2])
   || !parse_point(offset + 6, points[3])) {
    return 0;
  }

  rect.setTopLeft(points[0]);
  rect.setTopRight(points[1]);
  rect.setBottomRight(points[2]);
  rect.setBottomLeft(points[3]);

  return 8;
}

int Parser::parse_color(int offset, QColor& color) {

  qreal r = 0;
  qreal g = 0;
  qreal b = 0;
  qreal a = 0;

  if (!parse_real(offset + 0, r)
   || !parse_real(offset + 1, g)
   || !parse_real(offset + 2, b)
   || !parse_real(offset + 3, a)) {
    return 0;
  }

  color.setRedF(r);
  color.setGreenF(g);
  color.setBlueF(b);
  color.setAlphaF(a);

  return 4;
}

int Parser::parse_point(int offset, QPointF& point) {

  qreal a = 0;
  qreal b = 0;

  if (!parse_real(offset + 0, a)
   || !parse_real(offset + 1, b)) {
    return 0;
  }

  point.setX(a);
  point.setY(b);

  return 2;
}

int Parser::parse_int(int offset, int& value) {

  if (offset >= tokens->size()) {
    return 0;
  }

  if (!tokens->check_eq(offset, TokenType::Number)) {
    return 0;
  }

  bool ok = false;

  value = tokens->at(offset).data.toInt(&ok);

  return ok ? 1 : 0;
}

int Parser::parse_real(int offset, qreal& value) {

  if (offset >= tokens->size()) {
    return 0;
  }

  if (!tokens->check_eq(offset, TokenType::Number)) {
    return 0;
  }

  bool ok = false;

  value = tokens->at(offset).data.toDouble(&ok);

  return ok ? 1 : 0;
}

void Parser::on_token(TokenType type, const QStringRef& data) {
  tokens->add(type, data);
}
