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

add_library("herald-common" INTERFACE)

target_include_directories("herald-common" INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_options("herald-common" INTERFACE ${cxxflags})
