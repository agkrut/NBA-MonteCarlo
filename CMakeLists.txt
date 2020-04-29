cmake_minimum_required(VERSION 2.80)

set (CMAKE_CXX_FLAGS "-g -Wall -fopenmp")
set(CMAKE_CXX_STANDARD 11)

project(NBA-MonteCarlo)

include_directories(include)
include_directories(data)
file(GLOB SOURCES src/*.cpp)

add_executable(exec ${SOURCES})