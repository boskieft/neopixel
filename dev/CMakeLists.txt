cmake_minimum_required(VERSION 3.16)

# Treat the repository root as an ESP-IDF component while keeping this
# development application separate from the reusable component itself.
set(EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/..")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(neopixel_dev)
