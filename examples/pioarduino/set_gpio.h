#pragma once
#define SET_GPIO_H
#include <Arduino.h>
#if (CONFIG_IDF_TARGET_ESP32)
static const gpio_num_t enablePin = GPIO_NUM_26; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_19;   // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
#elif (CONFIG_IDF_TARGET_ESP32S2)
static const gpio_num_t enablePin = GPIO_NUM_5;
static const gpio_num_t dataPin = GPIO_NUM_9;
#elif (CONFIG_IDF_TARGET_ESP32S3)
static const gpio_num_t enablePin = GPIO_NUM_16;
static const gpio_num_t dataPin = GPIO_NUM_17;
#elif (CONFIG_IDF_TARGET_ESP32C3)
static const gpio_num_t enablePin = GPIO_NUM_10;
static const gpio_num_t dataPin = GPIO_NUM_5;
#elif (CONFIG_IDF_TARGET_ESP32C6)
static const gpio_num_t enablePin = GPIO_NUM_21;
static const gpio_num_t dataPin = GPIO_NUM_2;
#else
#error "ESP32xx target device has not been set, or is unknown"
#endif
