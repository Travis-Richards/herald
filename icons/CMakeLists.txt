cmake_minimum_required(VERSION 3.0)

find_package(Qt5 REQUIRED COMPONENTS Core)

set(CMAKE_AUTORCC ON)

add_library("herald-icons" "icons.qrc")
