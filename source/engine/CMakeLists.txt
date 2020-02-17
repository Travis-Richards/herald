cmake_minimum_required(VERSION 3.0)

if(NOT TARGET "herald-common")
  add_subdirectory("../common" "common")
endif(NOT TARGET "herald-common")

find_package(Qt5 COMPONENTS Widgets)

if(Qt5Widgets_FOUND)

  set(Qt_SOURCES
    "include/herald/QtEngine.h"
    "include/herald/QtTarget.h"
    "QtBackground.h"
    "QtBackground.cxx"
    "QtEngine.cxx"
    "QtModel.h"
    "QtModel.cxx"
    "QtObject.h"
    "QtObject.cxx"
    "QtObjectTable.h"
    "QtObjectTable.cxx"
    "QtRoom.h"
    "QtRoom.cxx"
    "QtTarget.cxx"
    "QtTextureTable.h"
    "QtTextureTable.cxx"
    "QtTile.h"
    "QtTile.cxx")

  set(Qt_LIBS Qt5::Widgets)

endif(Qt5Widgets_FOUND)

add_library("herald-engine"
  "include/herald/Action.h"
  "include/herald/ActionTable.h"
  "include/herald/Animation.h"
  "include/herald/AnimationTable.h"
  "include/herald/Background.h"
  "include/herald/Engine.h"
  "include/herald/Index.h"
  "include/herald/Model.h"
  "include/herald/Object.h"
  "include/herald/ObjectTable.h"
  "include/herald/Room.h"
  "include/herald/TextureTable.h"
  "include/herald/Tile.h"
  "ActionTable.cxx"
  "Animation.cxx"
  "AnimationTable.cxx"
  "Object.cxx"
  "ObjectTable.cxx"
  "Tile.cxx"
  ${Qt_SOURCES})

target_include_directories("herald-engine" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries("herald-engine" PRIVATE "herald-common")

target_link_libraries("herald-engine" PUBLIC ${Qt_LIBS})
