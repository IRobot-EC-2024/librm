cmake_minimum_required(VERSION 3.13)

if (NOT IROBOTEC_PLATFORM STREQUAL "STM32")
    # serial - https://github.com/wjwwood/serial - https://github.com/lunarifish/serial
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/serial)
endif ()

# pack up all third party libraries to a single target
add_library(third_party INTERFACE)
if (IROBOTEC_PLATFORM STREQUAL "LINUX")
    target_link_libraries(third_party INTERFACE serial)
endif ()