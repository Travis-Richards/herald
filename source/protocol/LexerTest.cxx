#include <gtest/gtest.h>

#include <herald/ScopedPtr.h>

#include <herald/protocol/Lexer.h>
#include <herald/protocol/Token.h>

#include <cstring>

using namespace herald;
using namespace herald::protocol;

namespace {

ScopedPtr<Lexer> make_lexer(const char* text) {
  return Lexer::make(text, std::strlen(text));
}

} // namespace

TEST(LexerTest, Scan) {

  auto lexer = make_lexer("_id1 id2\t-123'string 1'\"string 2\"\n\r\n\r");

  auto tok1 = lexer->scan();
  EXPECT_EQ(tok1.has_type(TokenType::Identifier), true);
  EXPECT_EQ(tok1.has_data("_id1"), true);

  auto tok2 = lexer->scan();
  EXPECT_EQ(tok2.has_type(TokenType::Space), true);
  EXPECT_EQ(tok2.has_data(" "), true);

  auto tok3 = lexer->scan();
  EXPECT_EQ(tok3.has_type(TokenType::Identifier), true);
  EXPECT_EQ(tok3.has_data("id2"), true);

  auto tok4 = lexer->scan();
  EXPECT_EQ(tok4.has_type(TokenType::Space), true);
  EXPECT_EQ(tok4.has_data("\t"), true);

  auto tok5 = lexer->scan();
  EXPECT_EQ(tok5.has_type(TokenType::NegativeSign), true);
  EXPECT_EQ(tok5.has_data("-"), true);

  auto tok6 = lexer->scan();
  EXPECT_EQ(tok6.has_type(TokenType::Number), true);
  EXPECT_EQ(tok6.has_data("123"), true);

  auto tok7 = lexer->scan();
  EXPECT_EQ(tok7.has_type(TokenType::StringLiteral), true);
  EXPECT_EQ(tok7.has_data("string 1"), true);

  auto tok8 = lexer->scan();
  EXPECT_EQ(tok8.has_type(TokenType::StringLiteral), true);
  EXPECT_EQ(tok8.has_data("string 2"), true);

  auto tok9 = lexer->scan();
  EXPECT_EQ(tok9.has_type(TokenType::Newline), true);
  EXPECT_EQ(tok9.has_data("\n"), true);

  auto tok10 = lexer->scan();
  EXPECT_EQ(tok10.has_type(TokenType::Newline), true);
  EXPECT_EQ(tok10.has_data("\r\n"), true);

  auto tok11 = lexer->scan();
  EXPECT_EQ(tok11.has_type(TokenType::Newline), true);
  EXPECT_EQ(tok11.has_data("\r"), true);
}
