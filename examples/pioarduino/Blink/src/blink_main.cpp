/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on pioarduino: Blink animation

    Blink one (1) Neopixel.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "neopixel.h"

static const char *TAG = "BLINK"; // for ESP_LOGx() logging

/*
===============================================================================
    Neopixel Configuration, adjust as needed for your own hardware setup
===============================================================================
*/
static const gpio_num_t dataPin = GPIO_NUM_5; // output data pin to DI of the Neopixels
static const size_t nrNeopixels = 1;          // just one (1) Neopixel to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;      // 3 colors (no white) in the usual GRB order, SEQ3 timing

/*
===============================================================================
    Start the Neopixel Driver
===============================================================================
*/
void setup(void) {
    // Logging
    esp_log_level_set("*", ESP_LOG_DEBUG); // enable ESP_LOGD debug messages
    delay(3000);                           // wait for Platformio monitor to connect

    ESP_LOGI(TAG, "----- Running setup, chip=`%s` -----", CONFIG_IDF_TARGET);

    // Start the Neopixel Driver
    if (!npx.begin(nrNeopixels, dataPin)) {
        ESP_LOGE(TAG, "STOPPED: Failed to start the Neopixel Driver");
        for (;;) {
            delay(100);
        }
    }

    // Preset the Neopixels
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness

    // Let's go
    ESP_LOGI(TAG, "Start the Blink animation");
}

/*
*******************************************************************************
    Main
*******************************************************************************
 */
void loop(void) {
    static bool isOn = true;

    if (isOn) {
        npx.setPixel(0, neopixelRed);
    } else {
        npx.setPixel(0, neopixelBlack);
    }
    npx.show();

    isOn = !isOn; // toggle for next iteration
    delay(100);   // for visibility, wait [ms] before the next iteration
}
