#include "Interpreter.h"

#include "ScopedPtr.h"

#include "lang/Lexer.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/SyntaxChecker.h"
#include "lang/Token.h"

#include <QString>
#include <QVector>

using namespace herald;

namespace herald {

namespace {

class NullInterpreter final : public Interpreter {
public:
  NullInterpreter(QObject* parent) : Interpreter(parent) {}
  bool interpret(Parser&) override {
    return true;
  }
};

} // namespace

} // namespace herald

Interpreter* Interpreter::make_null(QObject* parent) {
  return new NullInterpreter(parent);
}

bool Interpreter::check(const herald::parse_tree::Node& node) {

  auto errors = SyntaxErrorList::make();

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

  auto lexer = Lexer::make(text_std.c_str(), text_std.size());

  QVector<Token> tokens;

  while (!lexer->done()) {
    auto token = lexer->scan();
    if (!token.has_type(TokenType::Newline)
     && !token.has_type(TokenType::Space)) {
      tokens.push_back(token);
    }
  }

  auto parser = Parser::make(tokens.data(), tokens.size());

  auto success = interpret(*parser);

  return success;
}
