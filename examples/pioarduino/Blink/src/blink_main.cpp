/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on Pioarduino: Blink animation

    Blink both the Status LED (on standard GPIO) and one Neopixel at the same time.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "set_gpio.h" // comment-out to define the GPIO pins manually below
#include "neopixel.h"

static const char *TAG = "BLINK"; // for ESP_LOGx() logging

#ifndef SET_GPIO_H
// instead of using "set_gpio.h" you can also set the GPIO pins manually here
static const gpio_num_t dataPin = GPIO_NUM_5;      // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
static const gpio_num_t enablePin = GPIO_NUM_10;   // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t statusLedPin = GPIO_NUM_8; // optional output pin to drive the classic on/off Status LED
#endif

/*
===============================================================================
    Start the Neopixel driver and the Status LED
===============================================================================
*/
static const size_t nrNeopixels = 1; // just one (1) Neopixel to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;

void setup(void) {
    // Logging
    esp_log_level_set("*", ESP_LOG_DEBUG); // set log level to include ESP_LOGD messages
    delay(3000);                           // wait for Platformio monitor to connect

    ESP_LOGI(TAG, "----- Running setup, chip=`%s` -----", CONFIG_IDF_TARGET);

    // Optional: GPIO for Status LED output
    if (statusLedPin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Using statusLedPin=%d", statusLedPin);
        pinMode(statusLedPin, OUTPUT);
    } else {
        ESP_LOGW(TAG, "Optional statusLedPin is NOT configured, Status LED will not blink");
    }

    // GPIO for Data output
    if (dataPin == GPIO_NUM_NC) {
        ESP_LOGE(TAG, "STOPPED: No dataPin configured");
        for (;;) {
            delay(100); // wait indefinitely
        }
    }
    ESP_LOGI(TAG, "Using dataPin=%d", dataPin);

    // Optional: set Enable output to High
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, HIGH);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }

    // Init the driver
    ESP_LOGI(TAG, "Init the Neopixels on pin=%d with %d pixels", dataPin, nrNeopixels);
    if (!npx.begin(nrNeopixels, dataPin)) {
        ESP_LOGE(TAG, "STOPPED: init failed");
        for (;;) {
            delay(100); // wait indefinitely
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

    if (statusLedPin != GPIO_NUM_NC) {
        // Note: depending on the wiring of the Status LED, you may have to swap HIGH and LOW
        digitalWrite(statusLedPin, (isOn ? HIGH : LOW));
    } // else: status LED is not configured

    isOn = !isOn; // toggle for next iteration
    delay(100);   // for visibility, wait [ms] before the next iteration
}
