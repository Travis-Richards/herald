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
set(CMAKE_AUTORCC ON)

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
  "Controller.h"
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
  "KeyController.h"
  "KeyController.cxx"
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
  "RoomBuilder.h"
  "RoomBuilder.cxx"
  "SelectionIndex.h"
  "SettingsDialog.h"
  "SettingsDialog.cxx"
  "WorkQueue.h"
  "WorkQueue.cxx"
  "Writer.h"
  "Writer.cxx"
  "${PROJECT_SOURCE_DIR}/icons.qrc")

set_target_properties("herald-hub" PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

target_link_libraries("herald-hub" PRIVATE
  ${LIBS}
  "herald-common"
  "herald-engine"
  "herald-protocol")

target_include_directories("herald-hub" PRIVATE ${INCLUDE_DIRS})

target_compile_options("herald-hub" PRIVATE ${cxxflags})

install(TARGETS "herald-hub" DESTINATION "bin")

if(APPLE AND Qt5_FOUND)

  if(EXISTS "/usr/local/opt/qt")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
  endif(EXISTS "/usr/local/opt/qt")

  # Thank you RIP tutorial for this.

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(mac_deploy_qt macdeployqt HINTS "${qt_binary_dir}")

  add_custom_command(TARGET herald-hub POST_BUILD
      COMMAND "${mac_deploy_qt}" "$<TARGET_FILE_DIR:herald-hub>/../.." -always-overwrite -dmg
      COMMENT "Running macdeployqt...")

elseif(WIN32 AND Qt5_FOUND)

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(win_deploy_qt windeployqt HINTS "${qt_binary_dir}")

  set(pkg_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64")
  set(zip_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64.zip")

  add_custom_command(TARGET herald-hub POST_BUILD
      COMMAND set PATH=%PATH%$<SEMICOLON>${qt_binary_dir}
      COMMAND "${win_deploy_qt}" "$<TARGET_FILE:herald-hub>" --dir "${pkg_path}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:herald-hub>" "${pkg_path}/$<TARGET_FILE_NAME:herald-hub>"
      COMMAND "${CMAKE_COMMAND}" -E tar cfv "${zip_path}" --format=zip "${pkg_path}"
      COMMENT "Running windeployqt...")

endif(APPLE AND Qt5_FOUND)

