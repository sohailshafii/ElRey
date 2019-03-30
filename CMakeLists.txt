cmake_minimum_required (VERSION 2.6)
project (ElRey)

set (ElRey_VERSION_MAJOR 0)
set (ElRey_VERSION_MINOR 5)

set (CMAKE_CXX_STANDARD 11)

# header file to pass some CMake settings
# to source code
configure_file (
	"${PROJECT_SOURCE_DIR}/src/ElReyConfig.h.in"
	"${PROJECT_BINARY_DIR}/ElReyConfig.h"
	)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)

set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/MainWindow.cpp)

add_executable(ElRey ${SOURCE_FILES})

target_include_directories(ElRey
	PRIVATE
		"${SDL2_INCLUDE_DIR}"
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/src/")

target_link_libraries(ElRey ${SDL2_LIBRARY})

set(SOURCE_FILES_MC ${PROJECT_SOURCE_DIR}/srcMC/Main.cpp)

add_executable(McTest ${SOURCE_FILES_MC})

