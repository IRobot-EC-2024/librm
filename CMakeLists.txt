cmake_minimum_required(VERSION 3.27)
project(irobotEC)

# add third party libraries
add_subdirectory(libs)
# main target
add_subdirectory(src)
# link third party libraries
target_link_libraries(${PROJECT_NAME} PUBLIC third_party)