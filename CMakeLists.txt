#
# Copyright (c) 2024 XDU-IRobot
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.27)
project(irobotEC)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/detect_platform.cmake)

# add third party libraries
add_subdirectory(libs)
# main target
add_subdirectory(src)
# link third party libraries
target_link_libraries(${PROJECT_NAME} PUBLIC third_party)

target_compile_definitions(${PROJECT_NAME} PUBLIC -DIROBOTEC_PLATFORM_${IROBOTEC_PLATFORM})
if (DEFINED IROBOTEC_PLATFORM_LINUX_TYPE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC -DIROBOTEC_PLATFORM_LINUX_${IROBOTEC_PLATFORM_LINUX_TYPE})
endif ()
