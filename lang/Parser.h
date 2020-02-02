#pragma once

#include <QObject>

class Lexer;
class Node;
class TokenList;
class QColor;
class QPolygonF;
class QString;
class QStringRef;

enum class TokenType : int;

/// Used for parsing responses from
/// the game.
class Parser : public QObject {
  Q_OBJECT
public:
  /// Constructs a new parser instance.
  /// @param input The input string to be parsed.
  /// @param parent A pointer to the parent object.
  Parser(const QString& input, QObject* parent = nullptr);
  /// Parses a single line.
  /// @returns True if a statement was found, false otherwise.
  bool parse();
signals:
  /// Emitted when an arbitrary node
  /// is successfully parsed.
  void found_node(const Node& node);
  /// Emitted when an error is found.
  void found_error();
protected slots:
  /// Handles a token produced by the lexer.
  void on_token(TokenType type, const QStringRef& data);
protected:
  /// Parses for a background statement.
  bool parse_background_stmt();
  /// Parses for a color texture declaration.
  bool parse_color_texture_decl();
  /// Parses for a finish statement.
  bool parse_finish_stmt();
  /// Parses for an image texture statement.
  bool parse_image_texture_decl();
  /// Parses for a polygon object statement.
  bool parse_polygon_object_stmt();
  /// Parses for polygon coordinates
  /// until the end of the line is found.
  int parse_polygon(int offset, QPolygonF& polygon);
  /// Parses for a color.
  int parse_color(int offset, QColor& color);
  /// Parses for a 2D point.
  int parse_point(int offset, QPointF& point);
  /// Parses a real number.
  int parse_real(int offset, qreal& value);
  /// Parses an integer.
  int parse_int(int offset, int& value);
private:
  /// A pointer to the lexer
  /// used to generate tokens
  /// for the parser.
  Lexer* lexer;
  /// A group of tokens emitted from
  /// the lexer. They are accumulated
  /// on a line basis.
  TokenList* tokens;
};
