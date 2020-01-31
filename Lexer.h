#pragma once

#include <QObject>

class QChar;
class QString;
class QStringRef;

/// Enumerates the several types of tokens
/// that can be found by the lexer.
enum class TokenType : int {
  /// An identifier sequence.
  /// This includes: a-z, A-Z, '_'
  Identifier,
  /// Used when read error occurs
  /// or when an invalid token is found.
  Invalid,
  /// A newline sequence.
  Newline,
  /// A number token.
  Number,
  /// A space or tab sequence.
  /// This is usually ignored.
  Space,
  /// Either a single or double
  /// quoted string.
  StringLiteral,
  /// A single or double quoted
  /// string without a terminated
  /// quotation symbol.
  UnterminatedStringLiteral
};

/// Used for scanning tokens from a response protocol.
class Lexer : public QObject {
  Q_OBJECT
public:
  /// Constructs a lexer instance.
  /// @param input The string to tokenize.
  /// @param parent A pointer to the parent object.
  Lexer(const QString& input, QObject* parent = nullptr);
  /// Scans for a token.
  /// A signal is emitted if a token is found.
  /// @returns True if a token was found, false otherwise.
  bool scan();
signals:
  /// This signal is emitted when a token is found.
  /// @param type The type of the token that was found.
  /// @param data The token character data.
  void token_found(TokenType type, const QStringRef& data);
protected:
  /// Indicates whether or not an offset
  /// is out of bounds.
  /// @param offset The offset to check boundaries for.
  /// @returns True if the offset is out of bounds,
  /// false if it is not.
  bool out_of_bounds(int offset) const noexcept;
  /// Indicates whether or not the end of readable
  /// data has been reached.
  bool at_end();
  /// Gets the number of characters remaining to be scanned.
  int remaining() const noexcept;
  /// Gets a character from the input
  /// at a certain offset.
  QChar peek(int offset) const;
  /// Goes to the next series of characters
  /// from the current position.
  void next(int count) noexcept;
  /// Completes a newline token.
  bool complete_newline();
  /// Completes a number token.
  bool complete_number();
  /// Completes an identifier token.
  bool complete_identifier();
  /// Completes a space token.
  bool complete_space();
  /// Completes a string literal token.
  /// @param quote The quote character
  /// that begins the string literal.
  bool complete_string_literal(const QChar& quote);
  /// Completes a token and emits the signal.
  bool complete(TokenType type, int size);
private:
  /// The input being scanned.
  const QString& input;
  /// The position of the scanner
  /// along the input string.
  int pos;
};
