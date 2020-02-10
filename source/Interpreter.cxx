#include "Interpreter.h"

#include "lang/Lexer.h"
#include "lang/ParseTree.h"
#include "lang/Parser.h"
#include "lang/ScopedPtr.h"
#include "lang/SyntaxChecker.h"
#include "lang/Token.h"

#include <QString>
#include <QVector>

bool Interpreter::check(const parse_tree::Node& node) {

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

  emit finished();

  return success;
}
