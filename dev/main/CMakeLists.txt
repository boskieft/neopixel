file(GLOB SOURCES "*.cpp")

idf_component_register(
    SRCS ${SOURCES}
    INCLUDE_DIRS "."
    REQUIRES
        neopixel
        esp_driver_gpio
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall
    -Wextra
    -Wformat
    -Wformat=2
    -Wconversion
    -Wimplicit-fallthrough
    -Wshadow
    -Werror
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LOG_LOCAL_LEVEL=ESP_LOG_DEBUG)
