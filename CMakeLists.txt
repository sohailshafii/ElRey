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

AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Performance SOURCE_FILES) 
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Materials SOURCE_FILES) 
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Math SOURCE_FILES) 
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/SceneData SOURCE_FILES)  
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/WorldData SOURCE_FILES)   
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Sampling SOURCE_FILES) 
AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/src/Cameras SOURCE_FILES)  

add_executable(ElRey ${SOURCE_FILES})

target_include_directories(ElRey
	PRIVATE
		"${SDL2_INCLUDE_DIR}"
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_SOURCE_DIR}/src/"
		"${PROJECT_SOURCE_DIR}/src/Math"
		"${PROJECT_SOURCE_DIR}/src/Materials"
		"${PROJECT_SOURCE_DIR}/src/Performance"
		"${PROJECT_SOURCE_DIR}/src/WorldData"
		"${PROJECT_SOURCE_DIR}/src/Sampling"
		"${PROJECT_SOURCE_DIR}/src/Cameras"
		"${PROJECT_SOURCE_DIR}/src/SceneData")

target_link_libraries(ElRey ${SDL2_LIBRARY})

