cmake_minimum_required(VERSION 3.0.2)

find_package(Qt5 REQUIRED COMPONENTS Core Gui)

add_library("herald-lang" STATIC
  "Interpreter.h"
  "Interpreter.cxx"
  "Lexer.h"
  "Lexer.cxx"
  "Matrix.h"
  "Matrix.cxx"
  "ParseTree.h"
  "ParseTree.cxx"
  "Parser.h"
  "Parser.cxx"
  "Writer.h"
  "Writer.cxx")

target_link_libraries("herald-lang" PRIVATE Qt5::Core Qt5::Gui)

if (DEFINED cxxflags)
  target_compile_options("herald-lang" PRIVATE ${cxxflags})
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options("herald-lang" PRIVATE -Wall -Wextra -Werror -Wfatal-errors -std=c++11)
endif (DEFINED cxxflags)
