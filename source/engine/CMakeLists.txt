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
    "QtRoom.h"
    "QtRoom.cxx"
    "QtTarget.h"
    "QtTarget.cxx"
    "QtTextureTable.h"
    "QtTextureTable.cxx"
    "QtTile.h"
    "QtTile.cxx")

  set(Qt_LIBS Qt5::Widgets)

  set(Qt_DEMO ON)

endif(Qt5Widgets_FOUND)

add_library("herald-engine"
  "Animation.h"
  "Animation.cxx"
  "AnimationTable.h"
  "AnimationTable.cxx"
  "Background.h"
  "Controller.h"
  "DefaultEngine.h"
  "DefaultEngine.cxx"
  "DefaultModel.h"
  "DefaultModel.cxx"
  "DefaultRoom.h"
  "DefaultRoom.cxx"
  "Engine.h"
  "Index.h"
  "Model.h"
  "Room.h"
  "Target.h"
  "TextureTable.h"
  "Tile.h"
  "Tile.cxx"
  ${Qt_SOURCES})

target_link_libraries("herald-engine" PRIVATE "herald-common")

target_link_libraries("herald-engine" PUBLIC ${Qt_LIBS})

if(Qt_DEMO)

  add_executable("herald-engine-qt-demo" "QtDemo.cxx")

  target_link_libraries("herald-engine-qt-demo" PRIVATE "herald-engine" "herald-common")

endif(Qt_DEMO)
