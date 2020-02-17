cmake_minimum_required(VERSION 3.0.2)

project("herald" VERSION 0.3.0)

# Add components

add_subdirectory("source/common")
add_subdirectory("source/engine")
add_subdirectory("source/protocol")
add_subdirectory("source/toolkit")
add_subdirectory("source")

if(APPLE)

  if(EXISTS "/usr/local/opt/qt")
    list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt")
  endif(EXISTS "/usr/local/opt/qt")

  # Thank you RIP tutorial for this.

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(mac_deploy_qt macdeployqt HINTS "${qt_binary_dir}")

  add_custom_command(TARGET herald-hub POST_BUILD
      COMMAND "${mac_deploy_qt}" "$<TARGET_FILE_DIR:herald-hub>/../.." -always-overwrite
      COMMENT "Running macdeployqt...")

elseif(WIN32)

  get_target_property(qmake_executable Qt5::qmake IMPORTED_LOCATION)

  get_filename_component(qt_binary_dir "${qmake_executable}" DIRECTORY)

  find_program(win_deploy_qt windeployqt HINTS "${qt_binary_dir}")

  set(pkg_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64")
  set(zip_path "${CMAKE_CURRENT_BINARY_DIR}/herald-w64.zip")

  add_custom_command(TARGET herald POST_BUILD
      COMMAND set PATH=%PATH%$<SEMICOLON>${qt_binary_dir}
      COMMAND "${win_deploy_qt}" "$<TARGET_FILE:herald-hub>" --dir "${pkg_path}"
      COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:herald-hub>" "${pkg_path}/$<TARGET_FILE_NAME:herald-hub>"
      COMMAND "${CMAKE_COMMAND}" -E tar cfv "${zip_path}" --format=zip "${pkg_path}"
      COMMENT "Running windeployqt...")

endif(APPLE)

set (CPACK_PACKAGE_NAME "Herald")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game engine")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)
