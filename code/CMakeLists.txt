# For more information about build system see
# https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html
# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
#CXX_STANDARD 17 quer dizer que o código é em C++17
cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)

set(EXTRA_COMPONENT_DIRS "${CMAKE_SOURCE_DIR}/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(main)
