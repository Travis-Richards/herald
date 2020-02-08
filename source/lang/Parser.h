#pragma once

#include <QObject>

class BuildRoomResponse;
class FillObjectsResponse;
class SetBackgroundResponse;

class Lexer;
class Node;
class TokenList;
class QString;
class QStringRef;
class Matrix;

enum class TokenType : int;

/// Used for parsing responses from the game.
class Parser : public QObject {
  Q_OBJECT
public:
  /// Constructs a new parser instance.
  /// @param input The input string to be parsed.
  /// @param parent A pointer to the parent object.
  Parser(const QString& input, QObject* parent = nullptr);
  /// Parses the response to a "build room" command.
  BuildRoomResponse* parse_build_room_response();
  /// Parses the response to an "add objects" command.
  FillObjectsResponse* parse_fill_objects_response();
  /// Parses teh response to a "set background" command.
  SetBackgroundResponse* parse_set_background_response();
protected slots:
  /// Handles a token produced by the lexer.
  void on_token(TokenType type, const QStringRef& data);
protected:
  /// Prepares the token list for parsing.
  /// It fills the token list and gets rid
  /// of spaces and newline characters.
  void prepare_tokens();
  /// Parses a matrix of a specified size.
  int parse_matrix(int offset, Matrix& matrix);
  /// Parses a size specificiation.
  int parse_size(int offset, QSize& size);
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
