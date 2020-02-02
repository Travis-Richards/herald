#include "Interpreter.h"

#include "ParseTree.h"
#include "Parser.h"

Interpreter::Interpreter(QObject* parent) : QObject(parent), finish_reached(false) {

}

void Interpreter::interpret_line(const QString& text) {

  auto parser = new Parser(text, this);

  connect(parser, &Parser::found_node, this, &Interpreter::handle_node);

  parser->parse();
}

void Interpreter::handle_node(const Node& node) {
  node.accept(*this);
}
