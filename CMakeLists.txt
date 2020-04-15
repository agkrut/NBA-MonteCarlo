cmake_minimum_required(VERSION 3.00)

set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set (CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_STANDARD 11)

project(NBA-MonteCarlo)

include_directories(include)
include_directories(data)
file(GLOB SOURCES src/*.cpp)

add_executable(exec ${SOURCES})