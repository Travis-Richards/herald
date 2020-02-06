#include "Interpreter.h"

#include "Parser.h"

Interpreter::Interpreter(QObject* parent) : QObject(parent) {

}

void Interpreter::interpret_text(const QString& text) {

  Parser parser(text);

  interpret(parser);
}
