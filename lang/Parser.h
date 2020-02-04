#pragma once

#include <QObject>

class Lexer;
class Node;
class TokenList;
class QColor;
class QRectF;
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
  /// Parses for a draw box statement.
  bool parse_draw_box_stmt();
  /// Parses for a finish statement.
  bool parse_finish_stmt();
  /// Parses for an image texture statement.
  bool parse_image_texture_decl();
  /// Parses rectangle coordinates.
  /// @param offset The token offset to begin at.
  /// @param rect The rectangle instance to put the data into.
  int parse_rect(int offset, QRectF& rect);
  /// Parses for a color.
  /// @param offset The token offset to begin at.
  int parse_color(int offset, QColor& color);
  /// Parses for a 2D point.
  /// @param offset The token offset to begin at.
  int parse_point(int offset, QPointF& point);
  /// Parses a real number.
  /// @param offset The token offset to begin at.
  int parse_real(int offset, qreal& value);
  /// Parses an integer.
  /// @param offset The token offset to begin at.
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
