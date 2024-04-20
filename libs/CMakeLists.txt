cmake_minimum_required(VERSION 3.27)

# crc library - https://github.com/gityf/crc
add_subdirectory(crc)

# tinyfsm library[header only] - https://github.com/digint/tinyfsm
add_library(tinyfsm INTERFACE)
target_include_directories(tinyfsm INTERFACE tinyfsm/include)

# package all third party libraries to a single target
add_library(third_party INTERFACE)
target_link_libraries(third_party INTERFACE crc tinyfsm)