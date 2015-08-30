cmake_minimum_required(VERSION 3.2)

include(lib/simplicity/simplicity-common.cmake)

# Project
#########################
project(simplicity-editor)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib)

add_subdirectory(lib/simplicity-cef)
add_subdirectory(lib/simplicity)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

# Source
#########################
file(GLOB_RECURSE SRC_FILES src/main/c++/*.cpp src/main/c++/*.h)

# Target
#########################
add_executable(simplicity-editor ${SRC_FILES})
target_include_directories(simplicity-editor PRIVATE src/main/c++)

# Target Dependencies
#########################

# Simplicity
target_link_libraries(simplicity-editor simplicity-cef)
target_link_libraries(simplicity-editor simplicity)

# Dynamic Loading
target_link_libraries(simplicity-editor dl)

# Target Resources
#########################
add_custom_command(TARGET simplicity-editor PRE_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${PROJECT_SOURCE_DIR}/build/assets
				   COMMENT "simplicity::editor || Copying assets")
add_custom_command(TARGET simplicity-editor PRE_BUILD
				   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/src/main/css ${PROJECT_SOURCE_DIR}/build/src/main/css
				   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/src/main/html ${PROJECT_SOURCE_DIR}/build/src/main/html
				   COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/src/main/js ${PROJECT_SOURCE_DIR}/build/src/main/js
				   COMMENT "simplicity::editor || Copying source")
