cmake_minimum_required(VERSION 3.13)

# kalman - https://github.com/mherb/kalman -  https://github.com/SF4cl/kalman.git
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/kalman)

# eigen - https://eigen.tuxfamily.org
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/eigen)

# eternal - https://github.com/mapbox/eternal
add_library(eternal INTERFACE)
target_include_directories(eternal INTERFACE ${CMAKE_CURRENT_LIST_DIR}/eternal/include)

# pack up all third party libraries to a single target
add_library(third_party INTERFACE)
target_link_libraries(third_party INTERFACE Kalman eigen eternal)
