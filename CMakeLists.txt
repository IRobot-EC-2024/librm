cmake_minimum_required(VERSION 3.27)
project(irobotEC)

# add libraries
add_subdirectory(libs)
# redirect to ./src
add_subdirectory(src)

# link libraries
target_link_libraries(${PROJECT_NAME} PUBLIC crc)