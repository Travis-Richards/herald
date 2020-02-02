cmake_minimum_required(VERSION 3.0.2)

project("herald" VERSION 0.1.0)

# Detect compiler, adjust CXX flags

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set (cxxflags -Wall -Wextra -Werror -Wfatal-errors)
endif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

# Configure the build time header
# so that we know what APIs we can use.

configure_file(Config.h.in Config.h)

list(APPEND INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}")

# Make sure we have Qt on the system.

find_package(Qt5 REQUIRED
  COMPONENTS
    Core
    Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add components

add_subdirectory("lang")

# Here we declare the main program.

add_executable("herald" WIN32
  "main.cc"
  "Api.h"
  "Api.cxx"
  "CentralWidget.h"
  "CentralWidget.cxx"
  "ControlPanel.h"
  "ControlPanel.cxx"
  "Engine.h"
  "Engine.cxx"
  "GameInfo.h"
  "GameInfo.cxx"
  "GameList.h"
  "GameList.cxx"
  "GameListView.h"
  "GameListView.cxx"
  "LineBuffer.h"
  "LineBuffer.cxx"
  "MainWindow.h"
  "MainWindow.cxx"
  "MenuBuilder.h"
  "MenuBuilder.cxx"
  "ProcessApi.h"
  "ProcessApi.cxx"
  "Scene.h"
  "Scene.cxx"
  "SceneView.h"
  "SceneView.cxx"
  "Icons.qrc")

target_link_libraries("herald" PRIVATE Qt5::Widgets ${LIBS} "herald-lang")

target_include_directories("herald" PRIVATE ${INCLUDE_DIRS})

target_compile_options("herald" PRIVATE ${cxxflags})
