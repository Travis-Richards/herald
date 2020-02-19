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
    "QtKeyController.h"
    "QtKeyController.cxx"
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

set(JSON_DST "${CMAKE_CURRENT_BINARY_DIR}/Json.h")
set(JSON_URL "https://github.com/nlohmann/json/releases/download/v3.7.3/json.hpp")

if(NOT EXISTS "${JSON_DST}")
  file(DOWNLOAD "${JSON_URL}" "${JSON_DST}")
endif(NOT EXISTS "${JSON_DST}")

add_library("herald-engine"
  "include/herald/Action.h"
  "include/herald/ActionTable.h"
  "include/herald/Animation.h"
  "include/herald/AnimationTable.h"
  "include/herald/Background.h"
  "include/herald/Controller.h"
  "include/herald/Engine.h"
  "include/herald/Index.h"
  "include/herald/JsonModel.h"
  "include/herald/Model.h"
  "include/herald/Object.h"
  "include/herald/ObjectTable.h"
  "include/herald/Room.h"
  "include/herald/TextureTable.h"
  "include/herald/Tile.h"
  "ActionTable.cxx"
  "Animation.cxx"
  "AnimationTable.cxx"
  "JsonModel.cxx"
  "Object.cxx"
  "ObjectTable.cxx"
  "Tile.cxx"
  ${JSON_DST}
  ${Qt_SOURCES})

target_include_directories("herald-engine" PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories("herald-engine" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries("herald-engine" PRIVATE "herald-common")

target_link_libraries("herald-engine" PUBLIC ${Qt_LIBS})

find_package(GTest)

if (GTest_FOUND)

  add_executable("herald-engine-test"
    "JsonModelTest.cxx")

  target_link_libraries("herald-engine-test"
    PRIVATE
      "herald-common"
      "herald-engine"
      GTest::GTest
      GTest::Main)

  add_test(NAME "herald-engine-test"
    COMMAND $<TARGET_FILE:herald-engine-test>)

  enable_testing()

endif (GTest_FOUND)
