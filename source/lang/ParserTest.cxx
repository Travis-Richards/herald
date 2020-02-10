#include <gtest/gtest.h>

#include "Parser.h"
#include "ParseTree.h"
#include "ScopedPtr.h"
#include "Token.h"

TEST(Parser, ParseInteger) {

  std::vector<Token> tokens;
  tokens.emplace_back(TokenType::Number, "123", 3, 0);
  tokens.emplace_back(TokenType::Number, "456", 3, 0);
  tokens.emplace_back(TokenType::NegativeSign, "-", 1, 0);
  tokens.emplace_back(TokenType::Number, "789", 3, 0);

  auto parser = Parser::make(tokens.data(), tokens.size());
  auto a = parser->parse_integer();
  auto b = parser->parse_integer();
  auto c = parser->parse_integer();

  EXPECT_EQ(a.is_negative(), false);
  EXPECT_EQ(b.is_negative(), false);
  EXPECT_EQ(c.is_negative(), true);

  int a_value = 0;
  int b_value = 0;
  int c_value = 0;

  EXPECT_EQ(a.to_signed_value(a_value), true);
  EXPECT_EQ(b.to_signed_value(b_value), true);
  EXPECT_EQ(c.to_signed_value(c_value), true);

  EXPECT_EQ(a_value, 123);
  EXPECT_EQ(b_value, 456);
  EXPECT_EQ(c_value, -789);
}
