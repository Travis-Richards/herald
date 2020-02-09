#pragma once

#include <QObject>

template <typename T>
class ScopedPtr;

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
  ScopedPtr<BuildRoomResponse> parse_build_room_response();
  /// Parses the response to an "add objects" command.
  ScopedPtr<FillObjectsResponse> parse_fill_objects_response();
  /// Parses teh response to a "set background" command.
  ScopedPtr<SetBackgroundResponse> parse_set_background_response();
signals:
  /// This signal is emitted when an error
  /// occurs during parsing.
  /// @param msg A message describing the error.
  void error(const QString& msg);
protected slots:
  /// Handles a token produced by the lexer.
  void on_token(TokenType type, const QStringRef& data);
protected:
  /// Emits a failure message.
  /// @param msg A message describing the error.
  /// @returns Always returns false.
  bool fail(const QString& msg);
  /// Prepares the token list for parsing.
  /// It fills the token list and gets rid
  /// of spaces and newline characters.
  /// @returns True if there's tokens left over,
  /// false if there's not.
  bool prepare_tokens();
  /// Parses a matrix of a specified size.
  /// @returns True on success, false on failure.
  bool parse_matrix(Matrix& matrix);
  /// Parses a size specificiation.
  /// @returns True on success, false on failure.
  bool parse_size(QSize& size);
  /// Parses an integer.
  /// @returns True on success, false on failure.
  bool parse_int(int& value);
private:
  /// A pointer to the lexer
  /// used to generate tokens
  /// for the parser.
  Lexer* lexer;
  /// A group of tokens emitted from
  /// the lexer. They are accumulated
  /// on a line basis.
  TokenList* tokens;
  /// The position of the parser
  /// within the token list.
  int position;
};
