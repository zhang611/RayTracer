
# simple CMakeLists.txt for building the C++ code
# for the "Hello World" example

cmake_minimum_required(VERSION 3.10)
project(TinyRayTracer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set project name
set(PROJECT_NAME TinyRayTracer)
# get current path
set(PROJECT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
# set project build path
set(PROJECT_BUILD_PATH ${PROJECT_ROOT_DIR}/../build)
set(RESOURCE_DIR ${PROJECT_ROOT_DIR}/asset)

add_subdirectory(3rdparty)
add_subdirectory(source)

