cmake_minimum_required(VERSION 3.0)

if(NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif(NOT TARGET "herald-common")

find_package(Qt5 COMPONENTS Widgets)

if(Qt5Widgets_FOUND)

  set(Qt_SOURCES
    "QtBackground.h"
    "QtBackground.cxx"
    "QtEngine.h"
    "QtEngine.cxx"
    "QtModel.h"
    "QtModel.cxx"
    "QtObject.h"
    "QtObject.cxx"
    "QtObjectTable.h"
    "QtObjectTable.cxx"
    "QtRoom.h"
    "QtRoom.cxx"
    "QtTarget.h"
    "QtTarget.cxx"
    "QtTextureTable.h"
    "QtTextureTable.cxx"
    "QtTile.h"
    "QtTile.cxx")

  set(Qt_LIBS Qt5::Widgets)

endif(Qt5Widgets_FOUND)

add_library("herald-engine"
  "Action.h"
  "ActionTable.h"
  "ActionTable.cxx"
  "Animation.h"
  "Animation.cxx"
  "AnimationTable.h"
  "AnimationTable.cxx"
  "Background.h"
  "DefaultEngine.h"
  "DefaultEngine.cxx"
  "DefaultModel.h"
  "DefaultModel.cxx"
  "DefaultRoom.h"
  "DefaultRoom.cxx"
  "Engine.h"
  "Index.h"
  "Model.h"
  "Object.h"
  "Object.cxx"
  "ObjectTable.h"
  "ObjectTable.cxx"
  "Room.h"
  "Target.h"
  "TextureTable.h"
  "Tile.h"
  "Tile.cxx"
  ${Qt_SOURCES})

target_include_directories("herald-engine" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries("herald-engine" PRIVATE "herald-common")

target_link_libraries("herald-engine" PUBLIC ${Qt_LIBS})
