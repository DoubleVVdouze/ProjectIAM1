cmake_minimum_required(VERSION 3.6)
project(ProjectIA)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(Include)

file(GLOB Source/*.cpp SOURCE_FILES)
add_executable(ProjectIA main.cpp ${SOURCE_FILES})