cmake_minimum_required(VERSION 3.0.2)

if (NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif (NOT TARGET "herald-common")

add_library("herald-protocol" STATIC
  "include/herald/protocol/Command.h"
  "include/herald/protocol/ParseTree.h"
  "include/herald/protocol/Parser.h"
  "include/herald/protocol/SyntaxChecker.h"
  "Command.cxx"
  "Lexer.h"
  "Lexer.cxx"
  "ParseTree.cxx"
  "Parser.cxx"
  "SyntaxChecker.cxx"
  "Token.h"
  "Token.cxx")

target_link_libraries("herald-protocol" PRIVATE "herald-common")

target_include_directories("herald-protocol" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

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
