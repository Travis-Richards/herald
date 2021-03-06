cmake_minimum_required(VERSION 3.0)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(Git REQUIRED)

if(NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif(NOT TARGET "herald-common")

if(NOT TARGET "herald-engine")
  add_subdirectory("../engine" "engine")
endif(NOT TARGET "herald-engine")

if(NOT TARGET "herald-icons")
  add_subdirectory("../../icons" "icons")
endif(NOT TARGET "herald-icons")

if(NOT TARGET "herald-sysinfo")
  add_subdirectory("../sysinfo" "sysinfo")
endif(NOT TARGET "herald-sysinfo")

if(NOT EXISTS "QCodeEditor/CMakeLists.txt")

  set(submodule_path "${CMAKE_CURRENT_SOURCE_DIR}/QCodeEditor")

  execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init "${submodule_path}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    RESULT_VARIABLE GIT_CMD_ERROR)

  if(NOT ${GIT_CMD_ERROR} EQUAL 0)
    message(FATAL_ERROR "Failed to clone code editor")
  endif(NOT ${GIT_CMD_ERROR} EQUAL 0)

endif(NOT EXISTS "QCodeEditor/CMakeLists.txt")

add_subdirectory("QCodeEditor")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable("herald-toolkit" WIN32
  "main.cxx"
  "templates.qrc"
  "ActionEditor.h"
  "ActionEditor.cxx"
  "AnimationEditor.h"
  "AnimationEditor.cxx"
  "CodeEditor.h"
  "CodeEditor.cxx"
  "Console.h"
  "Console.cxx"
  "GameInfo.h"
  "GameList.h"
  "GameList.cxx"
  "GameProcess.h"
  "GameProcess.cxx"
  "GraphicsView.h"
  "GraphicsView.cxx"
  "JavaLanguage.h"
  "JavaLanguage.cxx"
  "Language.h"
  "Language.cxx"
  "Manager.h"
  "Manager.cxx"
  "ProcessQueue.h"
  "ProcessQueue.cxx"
  "ProjectManager.h"
  "ProjectManager.cxx"
  "Project.h"
  "Project.cxx"
  "ProjectView.h"
  "ProjectView.cxx"
  "PythonLanguage.h"
  "PythonLanguage.cxx"
  "RoomEditor.h"
  "RoomEditor.cxx"
  "RoomMediator.h"
  "RoomMediator.cxx"
  "RoomPropertiesView.h"
  "RoomPropertiesView.cxx"
  "RoomToolMediator.h"
  "RoomToolMediator.cxx"
  "RoomToolPanel.h"
  "RoomToolPanel.cxx"
  "RoomToolView.h"
  "RoomToolView.cxx"
  "RoomView.h"
  "RoomView.cxx"
  "SourceFile.h"
  "SourceFile.cxx"
  "SourceTreeModel.h"
  "SourceTreeModel.cxx"
  "StartupDialog.h"
  "StartupDialog.cxx"
  "TableButton.h"
  "TableButton.cxx"
  "TableEditor.h"
  "TableEditor.cxx"
  "TableModel.h"
  "TableModel.cxx"
  "TableView.h"
  "TableView.cxx"
  "TextureEditor.h"
  "TextureEditor.cxx"
  "TextureView.h"
  "TextureView.cxx")

target_link_libraries("herald-toolkit" PRIVATE
  Qt5::Widgets
  QCodeEditor
  "herald-common"
  "herald-engine"
  "herald-icons"
  "herald-sysinfo")

set_target_properties("herald-toolkit" PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

install(TARGETS "herald-toolkit"
  RUNTIME DESTINATION "bin")
