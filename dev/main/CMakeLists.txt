idf_component_register(
    SRCS "main.cpp"
    REQUIRES neopixel esp_driver_gpio
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE LOG_LOCAL_LEVEL=ESP_LOG_DEBUG)
