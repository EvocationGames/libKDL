# Copyright (c) 2020-2021 Tom Hancocks
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

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

########################################################################################################################
### Kestrel Development Kit Configuration
project(KestrelDevelopmentKit-Library)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_BINARY_DIR ${CMAKE_BUILD_DIR})
set(CMAKE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

########################################################################################################################
## libKDL
file(GLOB_RECURSE LIB_SOURCES
    "kdl/*.cpp"
)

add_library(KDL ${LIB_SOURCES})
target_include_directories(KDL PUBLIC .)

########################################################################################################################
## Test Target
add_executable(kdl-test test/test_target.cpp)
target_include_directories(kdl-test PUBLIC .)
target_link_libraries(kdl-test KDL)

########################################################################################################################
## Testing
enable_testing()

file(GLOB_RECURSE TEST_SUITE
    test/suite/*.sh
)

foreach(TRIGGER_FILE ${TEST_SUITE})
    string(REGEX REPLACE "^.*/test/suite/" "" TEST_NAME "${TRIGGER_FILE}")
    string(REGEX REPLACE "/(.+)\\.sh$" "" TEST_NAME "${TEST_NAME}")

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TRIGGER_FILE}
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endforeach()

add_custom_command(
    TARGET kdl-test
    POST_BUILD
    COMMAND ctest --output-on-failure
)