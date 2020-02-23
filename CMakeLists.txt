cmake_minimum_required(VERSION 3.0.2)

project("herald" VERSION 0.3.3)

# Find relevant packages

find_package(Qt5 COMPONENTS Core Widgets)

if (Qt5Core_FOUND OR Qt5Widgets_FOUND)
  set (Qt5_FOUND TRUE)
endif (Qt5Core_FOUND OR Qt5Widgets_FOUND)

# Add components

add_subdirectory("icons")
add_subdirectory("source/common")
add_subdirectory("source/engine")
add_subdirectory("source/protocol")
add_subdirectory("source/toolkit")
add_subdirectory("source")

set (CPACK_PACKAGE_NAME "Herald")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "A 2D game engine")
set (CPACK_PACKAGE_VENDOR "Taylor Holberton")
set (CPACK_PACKAGE_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_CONTACT "taylorcholberton@gmail.com")
include(CPack)
