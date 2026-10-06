# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

# Increase the log level globally (incl components like the Neopixel driver)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLOG_LOCAL_LEVEL=ESP_LOG_DEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLOG_LOCAL_LEVEL=ESP_LOG_DEBUG")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)
project(ESP-IDF-blink)
