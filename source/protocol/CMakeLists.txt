cmake_minimum_required(VERSION 3.0.2)

if (NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif (NOT TARGET "herald-common")

add_library("herald-protocol" STATIC
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

target_link_libraries("herald-protocol" PRIVATE "herald-common")

find_package(GTest)

if (GTest_FOUND)

  add_executable("herald-protocol-test"
    "LexerTest.cxx"
    "ParserTest.cxx"
    "SyntaxCheckerTest.cxx")

  target_link_libraries("herald-protocol-test" PRIVATE
    "herald-common"
    "herald-protocol"
    GTest::GTest
    GTest::Main)

  add_test(NAME "herald-protocol-test"
    COMMAND $<TARGET_FILE:herald-protocol-test>)

endif (GTest_FOUND)

enable_testing()
