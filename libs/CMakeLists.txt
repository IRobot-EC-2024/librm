cmake_minimum_required(VERSION 3.22)

# crc - https://github.com/gityf/crc
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/crc)

# tinyfsm[header only] - https://github.com/digint/tinyfsm
add_library(tinyfsm INTERFACE)
target_include_directories(tinyfsm INTERFACE ${CMAKE_CURRENT_LIST_DIR}/tinyfsm/include)

# tweeny - https://github.com/mobius3/tweeny
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tweeny)

# nanopb - https://github.com/nanopb/nanopb/
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/nanopb)

# Eigen[header only] - https://eigen.tuxfamily.org/
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/eigen)

# spdlog[header only] - https://github.com/gabime/spdlog/
if (NOT IROBOTEC_PLATFORM STREQUAL "STM32")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog)
endif ()

# pack up all third party libraries to a single target
add_library(third_party INTERFACE)
target_link_libraries(third_party INTERFACE crc tinyfsm tweeny nanopb eigen)
if (IROBOTEC_PLATFORM STREQUAL "LINUX")
    target_link_libraries(third_party INTERFACE spdlog::spdlog)
endif ()