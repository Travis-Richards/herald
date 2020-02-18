#include "Interpreter.h"

#include <herald/ScopedPtr.h>

#include <herald/protocol/Lexer.h>
#include <herald/protocol/ParseTree.h>
#include <herald/protocol/Parser.h>
#include <herald/protocol/SyntaxChecker.h>
#include <herald/protocol/Token.h>

#include <QString>
#include <QVector>

namespace herald {

namespace {

class NullInterpreter final : public Interpreter {
public:
  NullInterpreter(QObject* parent) : Interpreter(parent) {}
  bool interpret(protocol::Parser&) override {
    return true;
  }
};

} // namespace

} // namespace herald

Interpreter* Interpreter::make_null(QObject* parent) {
  return new herald::NullInterpreter(parent);
}

bool Interpreter::check(const herald::protocol::Node& node) {

  auto errors = herald::protocol::SyntaxErrorList::make();

  auto checker = make_syntax_checker(errors.get());

  node.accept(*checker);

  if (errors->size() == 0) {
    return true;
  }

  for (std::size_t i = 0; i < errors->size(); i++) {
    emit error(*errors->at(i));
  }

  return false;
}

bool Interpreter::interpret_text(const QString& text) {

  auto text_std = text.toStdString();

  auto lexer = herald::protocol::Lexer::make(text_std.c_str(), text_std.size());

  QVector<herald::protocol::Token> tokens;

  while (!lexer->done()) {
    auto token = lexer->scan();
    if (!token.has_type(herald::protocol::TokenType::Newline)
     && !token.has_type(herald::protocol::TokenType::Space)) {
      tokens.push_back(token);
    }
  }

  auto parser = herald::protocol::Parser::make(tokens.data(), tokens.size());

  auto success = interpret(*parser);

  return success;
}
