cmake_minimum_required(VERSION 2.8)
Project(LibFloorPlan)
get_filename_component(PROJECT_ROOT . ABSOLUTE)
# You can prepare verbose Makefile to see what's going.
set(CMAKE_VERBOSE_MAKEFILE false)
# Set build type
set(CMAKE_BUILD_TYPE Debug)

# set the output directory.
SET(OUTPUT ${PROJECT_ROOT}/output CACHE PATH "Path prefix for installation")
SET(CMAKE_BINARY_DIR ${OUTPUT})

add_subdirectory(./src/lib)
add_subdirectory(./src/test)
add_subdirectory(./src/genmatlab)