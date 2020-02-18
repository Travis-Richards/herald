#include <gtest/gtest.h>

#include <herald/ScopedPtr.h>

#include <herald/protocol/ParseTree.h>
#include <herald/protocol/SyntaxChecker.h>

#include "Token.h"

using namespace herald;
using namespace herald::protocol;

TEST(SyntaxChecker, MissingIntegerValue) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  Integer value(nullptr, nullptr);

  value.accept(*syntax_checker);

  ASSERT_EQ(syntax_errors->size(), 1);

  const auto* error = syntax_errors->at(0);

  EXPECT_EQ(error->get_id(), SyntaxErrorID::MissingIntegerValue);
}

TEST(SyntaxChecker, InvalidIntegerValue) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  Token value_token(TokenType::Identifier, "id", 2, 0);

  Integer value(nullptr, &value_token);

  value.accept(*syntax_checker);

  ASSERT_EQ(syntax_errors->size(), 1);

  const auto* error = syntax_errors->at(0);

  EXPECT_EQ(error->get_id(), SyntaxErrorID::InvalidIntegerValue);
}

TEST(SyntaxChecker, InvalidSignSymbol) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  Token value_token(TokenType::Number, "12", 2, 0);
  Token sign_token(TokenType::Identifier, "id", 2, 0);
  Integer value(&sign_token, &value_token);

  value.accept(*syntax_checker);

  ASSERT_EQ(syntax_errors->size(), 1);

  const auto* error = syntax_errors->at(0);

  EXPECT_EQ(error->get_id(), SyntaxErrorID::InvalidSignSymbol);
}

TEST(SyntaxChecker, GoodInteger1) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  Token value_token(TokenType::Number, "12", 2, 0);
  Integer value(nullptr, &value_token);

  value.accept(*syntax_checker);

  EXPECT_EQ(syntax_errors->size(), 0);
}

TEST(SyntaxChecker, GoodInteger2) {

  auto syntax_errors = SyntaxErrorList::make();

  auto syntax_checker = make_syntax_checker(syntax_errors.get());

  Token sign_token(TokenType::NegativeSign, "-", 1, 0);
  Token value_token(TokenType::Number, "12", 2, 0);
  Integer value(&sign_token, &value_token);

  value.accept(*syntax_checker);

  EXPECT_EQ(syntax_errors->size(), 0);
}
