if(DEFINED COMPONENT_DIR)
    idf_component_register(
        SRCS
            "src/neopixel.cpp"
            "src/neopixel_i2s.cpp"
        INCLUDE_DIRS
            "include"
        REQUIRES
            esp_driver_i2s
            esp_timer
    )
else()
    cmake_minimum_required(VERSION 3.16)

    # Allow this repository to be opened directly as the ESP-IDF project root
    # while keeping it consumable as a standalone component.
    set(EXTRA_COMPONENT_DIRS
        "${CMAKE_CURRENT_LIST_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}/dev/main"
    )

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(neopixel_dev)
endif()
