cmake_minimum_required(VERSION 3.2)

include(../simplicity/simplicity-common.cmake)

# Project
#########################
project(simplicity-editor)

# Source
#########################
file(GLOB_RECURSE SRC_FILES src/main/c++/*.cpp src/main/c++/*.h)

# Target
#########################
add_library(simplicity-editor STATIC ${SRC_FILES})
target_include_directories(simplicity-editor PUBLIC src/main/c++)

# Target Dependencies
#########################

# Simplicity
target_link_libraries(simplicity-editor simplicity)
target_link_libraries(simplicity-editor simplicity-cef)
