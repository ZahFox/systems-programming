cmake_minimum_required(VERSION 3.15)
project(lalg_demo C)

set(CMAKE_C_STANDARD 11)

include_directories(lib)

add_executable(lalg_demo main.c lib/vector.h lib/includes.h)