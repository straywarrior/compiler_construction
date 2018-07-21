cmake_minimum_required (VERSION 2.8)

project(tinycompiler)

add_definitions('-g')
add_definitions('-Wall')
add_definitions('-std=c++11')

set(source_list scanner.cpp)

add_library(${PROJECT_NAME} ${source_list})

add_subdirectory(test)
