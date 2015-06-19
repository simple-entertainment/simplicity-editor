cmake_minimum_required(VERSION 3.2)

include(../simplicity/simplicity-common.cmake)

# Project
project(simplicity-editor)

# Paths
file(GLOB_RECURSE SRC_FILES src/main/c++/*.cpp src/main/c++/*.h)
include_directories(../simplicity/src/main/c++)
include_directories(../simplicity-rocket/dep/librocket/include)
include_directories(../simplicity-rocket/src/main/c++)
include_directories(src/main/c++)

# Output
add_library(simplicity-editor STATIC ${SRC_FILES})
