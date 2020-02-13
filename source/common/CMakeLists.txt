cmake_minimum_required(VERSION 3.0)

# Detect compiler, adjust CXX flags

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))

  set (cxxflags -Wall -Wextra -Werror -Wfatal-errors -std=c++11)

endif ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))

# Declare common library.
# All libraries will link to
# this.

add_library("herald-common"
  "ScopedPtr.h")

target_include_directories("herald-common" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_compile_options("herald-common" PUBLIC ${cxxflags})
