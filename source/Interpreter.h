#pragma once

#include <QObject>

class QString;

namespace herald {

namespace protocol {

class Node;
class Parser;
class SyntaxError;

} // namespace protocol

} // namespace herald

/// Used for interpreting the response
/// of a game instance.
class Interpreter : public QObject {
  Q_OBJECT
public:
  /// Creates a null interpreter.
  /// This interpreter does nothing
  /// and mostly acts as a placeholder.
  /// @param parent A pointer to the parent object.
  static Interpreter* make_null(QObject* parent);
  /// Constructs the base interpreter instance.
  /// @param parent A pointer to the parent object.
  Interpreter(QObject* parent) : QObject(parent) {}
  /// Just a stub.
  virtual ~Interpreter() {}
  /// Tokenizes the line of text and relays
  /// the parser to a derived class.
  /// @param text The text from the response.
  bool interpret_text(const QString& text);
signals:
  /// This signal is emitted when a syntax
  /// error is detected by the parser.
  void error(const herald::protocol::SyntaxError& error);
protected:
  /// Checks an arbitrary syntax node.
  /// @param node The node to check.
  /// @returns True if there was no errors,
  /// false if there were errors.
  bool check(const herald::protocol::Node& node);
  /// Interprets the response using a parser.
  /// @param parser A parser instance to parse the response with.
  /// @returns True on success, false on failure.
  virtual bool interpret(herald::protocol::Parser& parser) = 0;
};
