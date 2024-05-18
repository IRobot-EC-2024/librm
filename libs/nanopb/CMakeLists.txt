cmake_minimum_required(VERSION 3.28)
project(nanopb)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.c")

add_library(nanopb STATIC ${SOURCES})
target_include_directories(nanopb PUBLIC "${CMAKE_CURRENT_LIST_DIR}")