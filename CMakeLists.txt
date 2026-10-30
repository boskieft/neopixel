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
    #--------------------------------------------------------------------------
    # Allow this repository to be opened directly as the ESP-IDF project root
    # while keeping it consumable as a standalone component.
    #--------------------------------------------------------------------------
    cmake_minimum_required(VERSION 3.16)

    set(EXTRA_COMPONENT_DIRS
        "${CMAKE_CURRENT_LIST_DIR}"
        "${CMAKE_CURRENT_LIST_DIR}/dev/main"
    )

    # Point to the sdkconfig.defaults* files in the `dev` folder (i/o root)
    set(SDKCONFIG_DEFAULTS "${CMAKE_CURRENT_LIST_DIR}/dev/sdkconfig.defaults")
    if(DEFINED IDF_TARGET)
        set(_target_defaults "${CMAKE_CURRENT_LIST_DIR}/dev/sdkconfig.defaults.${IDF_TARGET}")
        if(EXISTS "${_target_defaults}")
            list(APPEND SDKCONFIG_DEFAULTS "${_target_defaults}")
        endif()
    endif()

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(neopixel_dev)
endif()
