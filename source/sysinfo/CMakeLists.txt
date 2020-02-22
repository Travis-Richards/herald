cmake_minimum_required(VERSION 3.0)

if(NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif(NOT TARGET "herald-common")

add_library("herald-sysinfo"
  "include/herald/sysinfo/ProgramFinder.h"
  "include/herald/sysinfo/Registry.h"
  "ProgramFinder.cxx"
  "Registry.cxx")

target_include_directories("herald-sysinfo" PUBLIC "include")

target_link_libraries("herald-sysinfo" PRIVATE "herald-common")

add_executable("herald-sysinfo-example" "Example.cxx")

target_link_libraries("herald-sysinfo-example" PRIVATE "herald-common" "herald-sysinfo")

set_target_properties("herald-sysinfo-example" PROPERTIES OUTPUT_NAME "example")
