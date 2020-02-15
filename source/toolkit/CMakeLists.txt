cmake_minimum_required(VERSION 3.0)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

if (NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif (NOT TARGET "herald-common")

if (NOT TARGET "herald-engine")
  add_subdirectory("../engine" "engine")
endif (NOT TARGET "herald-engine")

add_executable("herald-toolkit" WIN32
  "main.cxx"
  "ProjectView.h"
  "ProjectView.cxx"
  "StartupDialog.h"
  "StartupDialog.cxx")

target_link_libraries("herald-toolkit" PRIVATE
  Qt5::Widgets
  "herald-common"
  "herald-engine")

set_target_properties("herald-toolkit" PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
