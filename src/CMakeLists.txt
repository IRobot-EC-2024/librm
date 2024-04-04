cmake_minimum_required(VERSION 3.27)
project(irobotEC)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

add_library(irobotEC STATIC ${SOURCES})
target_include_directories(irobotEC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})