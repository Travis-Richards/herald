cmake_minimum_required(VERSION 3.0.2)

if (NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif (NOT TARGET "herald-common")

add_library("herald-lang" STATIC
  "Command.h"
  "Command.cxx"
  "Lexer.h"
  "Lexer.cxx"
  "ParseTree.h"
  "ParseTree.cxx"
  "Parser.h"
  "Parser.cxx"
  "SyntaxChecker.h"
  "SyntaxChecker.cxx"
  "Token.h"
  "Token.cxx")

target_link_libraries("herald-lang" PRIVATE "herald-common")

find_package(GTest)

if (GTest_FOUND)

  add_executable("herald-lang-test"
    "LexerTest.cxx"
    "ParserTest.cxx"
    "SyntaxCheckerTest.cxx")

  target_link_libraries("herald-lang-test" PRIVATE
    "herald-common"
    "herald-lang"
    GTest::GTest
    GTest::Main)

  add_test(NAME "herald-lang-test"
    COMMAND $<TARGET_FILE:herald-lang-test>)

endif (GTest_FOUND)

enable_testing()
