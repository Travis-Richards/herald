#include <gtest/gtest.h>

#include <herald/ScopedPtr.h>

#include <herald/protocol/Parser.h>
#include <herald/protocol/ParseTree.h>

#include "Token.h"

using namespace herald;
using namespace herald::protocol;

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

TEST(Parser, ParserSize) {

  std::vector<Token> tokens;
  tokens.emplace_back(TokenType::Number, "123", 3, 0);
  tokens.emplace_back(TokenType::Number, "456", 3, 0);

  auto parser = Parser::make(tokens.data(), tokens.size());

  auto size = parser->parse_size();

  EXPECT_EQ(size.valid(), true);

  int w = 0;
  int h = 0;

  EXPECT_EQ(size.to_values(w, h), true);
  EXPECT_EQ(w, 123);
  EXPECT_EQ(h, 456);
}

TEST(Parser, ParseMatrix) {

  std::vector<Token> tokens;
  tokens.emplace_back(TokenType::Number, "2", 1, 0);
  tokens.emplace_back(TokenType::Number, "3", 1, 0);
  tokens.emplace_back(TokenType::Number, "111", 3, 0);
  tokens.emplace_back(TokenType::Number, "222", 3, 0);
  tokens.emplace_back(TokenType::Number, "333", 3, 0);
  tokens.emplace_back(TokenType::Number, "444", 3, 0);
  tokens.emplace_back(TokenType::Number, "555", 3, 0);
  tokens.emplace_back(TokenType::Number, "666", 3, 0);

  auto parser = Parser::make(tokens.data(), tokens.size());

  auto matrix = parser->parse_matrix();

  auto size = matrix->get_size();
  EXPECT_EQ(size.valid(), true);

  int w = 0;
  int h = 0;

  EXPECT_EQ(size.to_values(w, h), true);
  EXPECT_EQ(w, 2);
  EXPECT_EQ(h, 3);

  EXPECT_EQ(matrix->get_integer_count(), 6);

  auto a = matrix->get_integer(0);
  auto b = matrix->get_integer(1);
  auto c = matrix->get_integer(2);
  auto d = matrix->get_integer(3);
  auto e = matrix->get_integer(4);
  auto f = matrix->get_integer(5);

  int values[6] = {
    0, 0, 0,
    0, 0, 0
  };

  EXPECT_EQ(a.to_signed_value(values[0]), true);
  EXPECT_EQ(b.to_signed_value(values[1]), true);
  EXPECT_EQ(c.to_signed_value(values[2]), true);
  EXPECT_EQ(d.to_signed_value(values[3]), true);
  EXPECT_EQ(e.to_signed_value(values[4]), true);
  EXPECT_EQ(f.to_signed_value(values[5]), true);

  EXPECT_EQ(values[0], 111);
  EXPECT_EQ(values[1], 222);
  EXPECT_EQ(values[2], 333);
  EXPECT_EQ(values[3], 444);
  EXPECT_EQ(values[4], 555);
  EXPECT_EQ(values[5], 666);
}

TEST(Parser, ParseSetActionStmt) {

  std::vector<Token> tokens;
  tokens.emplace_back(TokenType::Identifier, "set_action", sizeof("set_action") - 1, 0);
  tokens.emplace_back(TokenType::Number, "4", 1, 0);
  tokens.emplace_back(TokenType::Number, "5", 1, 0);

  auto parser = Parser::make(tokens.data(), tokens.size());

  auto set_action_stmt = parser->parse_set_action_stmt();

  ASSERT_EQ(!!set_action_stmt, true);

  auto action_id = set_action_stmt->get_action_id();
  auto object_id = set_action_stmt->get_object_id();

  EXPECT_EQ(action_id.valid(), true);
  EXPECT_EQ(object_id.valid(), true);
}
