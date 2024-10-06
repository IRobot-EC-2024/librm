cmake_minimum_required(VERSION 3.13)

if (NOT LIBRM_PLATFORM STREQUAL "STM32")
    # serial - https://github.com/wjwwood/serial - https://github.com/lunarifish/serial
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/serial)
endif ()

# kalman - https://github.com/mherb/kalman -  https://github.com/SF4cl/kalman.git
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/kalman)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/eigen)

# pack up all third party libraries to a single target
add_library(third_party INTERFACE)
if (LIBRM_PLATFORM STREQUAL "LINUX")
    target_link_libraries(third_party INTERFACE serial)
endif ()

target_link_libraries(third_party INTERFACE Kalman eigen)