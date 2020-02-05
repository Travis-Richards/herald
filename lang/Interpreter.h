#pragma once

#include <QObject>

class QString;

/// An interpreter is used to handle
/// the response of the game when
/// issued a command.
class Interpreter : public QObject {
public:
  /// Constructs the base interpreter.
  /// @param object A pointer to the parent object.
  Interpreter(QObject* parent);
  /// Just a stub.
  virtual ~Interpreter() {}
  /// Interprets the response sent from the game.
  /// @param text The text to interpret from the game.
  virtual void interpret_text(const QString& text);
  /// Interprets the input using a parser instance.
  virtual void interpret(Parser& parser) = 0;
};
