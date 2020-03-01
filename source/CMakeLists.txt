cmake_minimum_required(VERSION 3.0)

# Configure the build time header
# so that we know what APIs we can use.

configure_file("Config.h.in" "Config.h")

list(APPEND INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}")

# Make sure we have Qt on the system.

find_package(Qt5 REQUIRED
  COMPONENTS
    Core
    Widgets)

list(APPEND LIBS
  Qt5::Core
  Qt5::Widgets)

set(CMAKE_AUTOMOC ON)

# Make sure we have the dependencies

if(NOT TARGET "herald-icons")
  add_subdirectory("../icons" "icons")
endif(NOT TARGET "herald-icons")

# Here we declare the main program.

add_executable("herald-hub" WIN32
  "main.cxx"
  "ActiveGame.h"
  "ActiveGame.cxx"
  "ActiveGameList.h"
  "ActiveGameList.cxx"
  "Api.h"
  "BackgroundModifier.h"
  "BackgroundModifier.cxx"
  "CentralWidget.h"
  "CentralWidget.cxx"
  "ControlPanel.h"
  "ControlPanel.cxx"
  "ErrorLog.h"
  "ErrorLog.cxx"
  "GameInfo.h"
  "GameInfo.cxx"
  "GameList.h"
  "GameList.cxx"
  "GameListView.h"
  "GameListView.cxx"
  "Interpreter.h"
  "Interpreter.cxx"
  "LegacyModelLoader.h"
  "LegacyModelLoader.cxx"
  "LineBuffer.h"
  "LineBuffer.cxx"
  "MainWindow.h"
  "MainWindow.cxx"
  "Manager.h"
  "Manager.cxx"
  "Matrix.h"
  "Matrix.cxx"
  "MenuBuilder.h"
  "MenuBuilder.cxx"
  "ModelLoader.h"
  "ModelLoader.cxx"
  "ObjectMapBuilder.h"
  "ObjectMapBuilder.cxx"
  "PathSetting.h"
  "PathSetting.cxx"
  "ProcessApi.h"
  "ProcessApi.cxx"
  "ResponseHandler.h"
  "ResponseHandler.cxx"
  "RoomBuilder.h"
  "RoomBuilder.cxx"
  "SelectionIndex.h"
  "SettingsDialog.h"
  "SettingsDialog.cxx"
  "WorkQueue.h"
  "WorkQueue.cxx"
  "Writer.h"
  "Writer.cxx")

set_target_properties("herald-hub" PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

target_link_libraries("herald-hub" PRIVATE
  ${LIBS}
  "herald-common"
  "herald-icons"
  "herald-engine"
  "herald-protocol")

target_include_directories("herald-hub" PRIVATE ${INCLUDE_DIRS})

target_compile_options("herald-hub" PRIVATE ${cxxflags})

install(TARGETS "herald-hub"
  RUNTIME DESTINATION "bin")
