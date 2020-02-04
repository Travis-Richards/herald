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
    Widgets
    3DCore
    3DExtras
    3DRender)

list(APPEND LIBS
  Qt5::Core
  Qt5::Widgets
  Qt5::3DCore
  Qt5::3DExtras
  Qt5::3DRender)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add components

add_subdirectory("lang")

# Here we declare the main program.

add_executable("herald" WIN32
  "main.cxx"
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
  "MaterialList.h"
  "MaterialList.cxx"
  "Icons.qrc")

target_link_libraries("herald" PRIVATE ${LIBS} "herald-lang")

target_include_directories("herald" PRIVATE ${INCLUDE_DIRS})

target_compile_options("herald" PRIVATE ${cxxflags})

install(TARGETS "herald" DESTINATION "bin")

if (APPLE)

  if (EXISTS "/usr/local/opt/qt")
    list (APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
  endif (EXISTS "/usr/local/opt/qt")

  # Thank you RIP tutorial for this.

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(mac_deploy_qt macdeployqt HINTS "${qt_binary_dir}")

  add_custom_command(TARGET herald POST_BUILD
      COMMAND "${mac_deploy_qt}" "$<TARGET_FILE_DIR:herald>/../.." -always-overwrite
      COMMENT "Running macdeployqt...")

elseif (WIN32)

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(win_deploy_qt windeployqt HINTS "${qt_binary_dir}")

  set (pkg_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64")
  set (zip_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64.zip")

  add_custom_command(TARGET herald POST_BUILD
      COMMAND set PATH=%PATH%$<SEMICOLON>${qt_binary_dir}
      COMMAND "${win_deploy_qt}" "$<TARGET_FILE:herald>" --dir "${pkg_path}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:herald>" "${pkg_path}/$<TARGET_FILE_NAME:herald>"
      COMMAND "${CMAKE_COMMAND}" -E tar cfv "${zip_path}" --format=zip "${pkg_path}"
      COMMENT "Running windeployqt...")

endif (APPLE)

set (CPACK_PACKAGE_NAME "Herald")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game engine")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)
