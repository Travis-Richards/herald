#pragma once

#include <QObject>

class Lexer;
class TokenList;
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
  /// Emitted when a background statement is found.
  void found_background(int texture_id);
  /// Emitted when a "done" statement is found.
  void found_finish();
  /// Emitted when an image texture is found.
  /// @param path The path to the image texture.
  void found_image_texture(const QString& path);
  /// Emitted when an error is found.
  void found_error();
protected slots:
  /// Handles a token produced by the lexer.
  void on_token(TokenType type, const QStringRef& data);
protected:
  /// Parses for a background statement.
  bool parse_background_stmt();
  /// Parses for a finish statement.
  bool parse_finish_stmt();
  /// Parses for an image texture statement.
  bool parse_image_texture_stmt();
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
