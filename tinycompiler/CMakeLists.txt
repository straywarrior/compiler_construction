cmake_minimum_required (VERSION 2.8)

project(tinycompiler)

add_definitions('-g')
add_definitions('-Wall')
add_definitions('-std=c++11')

set(source_list scanner.cpp parser.cpp)

add_library(tinycompiler ${source_list})
add_executable(tiny ${source_list} main.cpp)

add_subdirectory(test)
