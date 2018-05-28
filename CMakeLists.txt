cmake_minimum_required (VERSION 2.6)
project (ElRey)

set (ElRey_VERSION_MAJOR 0)
set (ElRey_VERSION_MINOR 5)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/MainWindow.cpp)

add_executable(ElRey ${SOURCE_FILES})
target_link_libraries(ElRey ${SDL2_LIBRARY})
