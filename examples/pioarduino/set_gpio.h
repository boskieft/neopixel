#pragma once
#define SET_GPIO_H
/*
***************************************************************************************************
    Set the GPIO pins for ESP32xx target devices

    Specifically for my own hardware setup, adjust to yours as needed.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>

#if (CONFIG_IDF_TARGET_ESP32)
static const gpio_num_t dataPin = GPIO_NUM_19;      // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
static const gpio_num_t enablePin = GPIO_NUM_26;    // optional: output pin to enable the 74HCT126 level shifter
static const gpio_num_t statusLedPin = GPIO_NUM_16; // optional: output pin to drive the classic on/off Status LED
#elif (CONFIG_IDF_TARGET_ESP32S2)
static const gpio_num_t dataPin = GPIO_NUM_9;
static const gpio_num_t enablePin = GPIO_NUM_5;
static const gpio_num_t statusLedPin = GPIO_NUM_15;
#elif (CONFIG_IDF_TARGET_ESP32S3)
static const gpio_num_t dataPin = GPIO_NUM_17;
static const gpio_num_t enablePin = GPIO_NUM_16;
static const gpio_num_t statusLedPin = GPIO_NUM_12;
#elif (CONFIG_IDF_TARGET_ESP32C3)
static const gpio_num_t dataPin = GPIO_NUM_5;
static const gpio_num_t enablePin = GPIO_NUM_10;
static const gpio_num_t statusLedPin = GPIO_NUM_8;
#elif (CONFIG_IDF_TARGET_ESP32C6)
static const gpio_num_t dataPin = GPIO_NUM_2;
static const gpio_num_t enablePin = GPIO_NUM_21;
static const gpio_num_t statusLedPin = GPIO_NUM_15;
#else
#error "[env:esp32xx] has not been set in Platformio.ini for this ESP32xx variant, or corresponding `CONFIG_IDF_TARGET_ESP32xx` is unknown here"
#endif
