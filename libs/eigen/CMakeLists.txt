cmake_minimum_required(VERSION 3.8)
project(eigen)

add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE ${CMAKE_CURRENT_LIST_DIR})

