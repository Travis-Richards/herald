cmake_minimum_required(VERSION 3.0.2)

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

if (DEFINED cxxflags)
  target_compile_options("herald-lang" PRIVATE ${cxxflags})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options("herald-lang" PRIVATE -Wall -Wextra -Werror -Wfatal-errors -std=c++11)
endif (DEFINED cxxflags)

find_package(GTest)

if (GTest_FOUND)

  add_executable("herald-lang-test"
    "LexerTest.cxx"
    "ParserTest.cxx"
    "SyntaxCheckerTest.cxx")

  target_link_libraries("herald-lang-test" PRIVATE
    "herald-lang"
    GTest::GTest
    GTest::Main)

  add_test(NAME "herald-lang-test"
    COMMAND $<TARGET_FILE:herald-lang-test>)

endif (GTest_FOUND)

enable_testing()
