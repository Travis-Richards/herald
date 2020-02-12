cmake_minimum_required(VERSION 3.0.2)

project("herald" VERSION 0.3.0)

# Detect compiler, adjust CXX flags

if ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))

  set (cxxflags -Wall -Wextra -Werror -Wfatal-errors -std=c++11)

endif ((CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))

# Configure the build time header
# so that we know what APIs we can use.

configure_file("source/Config.h.in" "Config.h")

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
set(CMAKE_AUTORCC ON)

# Add components

add_subdirectory("source/lang")

# Here we declare the main program.

add_executable("herald" WIN32
  "source/main.cxx"
  "source/Action.h"
  "source/Action.cxx"
  "source/ActionList.cxx"
  "source/ActionList.h"
  "source/ActiveGame.h"
  "source/ActiveGame.cxx"
  "source/ActiveGameList.h"
  "source/ActiveGameList.cxx"
  "source/Api.h"
  "source/Background.h"
  "source/BackgroundView.h"
  "source/BackgroundView.cxx"
  "source/BackgroundModifier.h"
  "source/BackgroundModifier.cxx"
  "source/CentralWidget.h"
  "source/CentralWidget.cxx"
  "source/ControlPanel.h"
  "source/ControlPanel.cxx"
  "source/Controller.h"
  "source/Engine.h"
  "source/Engine.cxx"
  "source/ErrorLog.h"
  "source/ErrorLog.cxx"
  "source/GameInfo.h"
  "source/GameInfo.cxx"
  "source/GameList.h"
  "source/GameList.cxx"
  "source/GameListView.h"
  "source/GameListView.cxx"
  "source/Interpreter.h"
  "source/Interpreter.cxx"
  "source/KeyController.h"
  "source/KeyController.cxx"
  "source/LineBuffer.h"
  "source/LineBuffer.cxx"
  "source/MainWindow.h"
  "source/MainWindow.cxx"
  "source/Matrix.h"
  "source/Matrix.cxx"
  "source/MenuBuilder.h"
  "source/MenuBuilder.cxx"
  "source/Object.h"
  "source/Object.cxx"
  "source/ObjectMap.h"
  "source/ObjectMap.cxx"
  "source/ObjectMapBuilder.h"
  "source/ObjectMapBuilder.cxx"
  "source/ObjectMapView.h"
  "source/ObjectMapView.cxx"
  "source/PathSetting.h"
  "source/PathSetting.cxx"
  "source/ProcessApi.h"
  "source/ProcessApi.cxx"
  "source/Room.h"
  "source/Room.cxx"
  "source/RoomBuilder.h"
  "source/RoomBuilder.cxx"
  "source/RoomView.h"
  "source/RoomView.cxx"
  "source/Scene.h"
  "source/Scene.cxx"
  "source/SceneView.h"
  "source/SceneView.cxx"
  "source/SelectionIndex.h"
  "source/SettingsDialog.h"
  "source/SettingsDialog.cxx"
  "source/Texture.h"
  "source/Texture.cxx"
  "source/TextureAnimation.h"
  "source/TextureAnimation.cxx"
  "source/TextureAnimationGroup.h"
  "source/TextureAnimationGroup.cxx"
  "source/Tile.h"
  "source/Writer.h"
  "source/Writer.cxx"
  "icons.qrc")

target_link_libraries("herald" PRIVATE ${LIBS} "herald-lang")

target_include_directories("herald" PRIVATE ${INCLUDE_DIRS})

target_compile_options("herald" PRIVATE ${cxxflags})

install(TARGETS "herald" DESTINATION "bin")

if(APPLE)

  if(EXISTS "/usr/local/opt/qt")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
  endif(EXISTS "/usr/local/opt/qt")

  # Thank you RIP tutorial for this.

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(mac_deploy_qt macdeployqt HINTS "${qt_binary_dir}")

  add_custom_command(TARGET herald POST_BUILD
      COMMAND "${mac_deploy_qt}" "$<TARGET_FILE_DIR:herald>/../.." -always-overwrite
      COMMENT "Running macdeployqt...")

elseif(WIN32)

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(win_deploy_qt windeployqt HINTS "${qt_binary_dir}")

  set(pkg_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64")
  set(zip_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64.zip")

  add_custom_command(TARGET herald POST_BUILD
      COMMAND set PATH=%PATH%$<SEMICOLON>${qt_binary_dir}
      COMMAND "${win_deploy_qt}" "$<TARGET_FILE:herald>" --dir "${pkg_path}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:herald>" "${pkg_path}/$<TARGET_FILE_NAME:herald>"
      COMMAND "${CMAKE_COMMAND}" -E tar cfv "${zip_path}" --format=zip "${pkg_path}"
      COMMENT "Running windeployqt...")

endif(APPLE)

set (CPACK_PACKAGE_NAME "Herald")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game engine")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)
