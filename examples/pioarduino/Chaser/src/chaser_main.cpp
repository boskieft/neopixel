/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on Pioarduino: Chaser animation

    The Neopixels will be lit one after each other

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "set_gpio.h" // comment-out to define the GPIO pins manually below
#include "neopixel.h"

static const char *TAG = "CHASER"; // for ESP_LOGx() logging

#ifndef SET_GPIO_H
// instead of using "set_gpio.h" you can also set the GPIO pins manually here
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
#endif

/*
-------------------------------------------------------------------------------
    Start the Neopixel driver
-------------------------------------------------------------------------------
*/
#define PIXEL_COUNT 24 // nr of Neopixels to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;

void setup(void) {
    esp_log_level_set("*", ESP_LOG_DEBUG); // set log level to include ESP_LOGD messages
    Serial.begin(115200);                  // for library debug output, and for Platformio monitor
    Serial.setDebugOutput(true);           // enable debug output to serial, for library debug output
    delay(5000);                           // wait for Platformio monitor to open

    ESP_LOGI(TAG, "----- Running setup, chip=`%s` -----", CONFIG_IDF_TARGET);

    if (dataPin == GPIO_NUM_NC) {
        ESP_LOGE(TAG, "STOPPED: No dataPin configured");
        for (;;) {
            delay(100); // wait indefinitely
        }
    }

    ESP_LOGI(TAG, "Using dataPin=%d", dataPin);
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, HIGH);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }

    ESP_LOGI(TAG, "Init the Neopixels on pin=%d with %d pixels", dataPin, PIXEL_COUNT);
    if (!npx.begin(PIXEL_COUNT, dataPin)) {
        ESP_LOGE(TAG, "STOPPED: init failed");
        for (;;) {
            delay(100); // wait indefinitely
        }
    }

    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
}

/*
-------------------------------------------------------------------------------
    Chaser animation
-------------------------------------------------------------------------------
*/
void chaserAnimation(void) {
    static int coloredIndex = 0;
    static int blackIndex = PIXEL_COUNT - 1;

    npx.setPixel(blackIndex, neopixelBlack); // erase previously colored pixel
    npx.setPixel(coloredIndex, neopixelRed); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= PIXEL_COUNT) {
        coloredIndex = 0; // new loop
    }
}

/*
***********************************************************
    Main
***********************************************************
 */
void loop(void) {

    ESP_LOGI(TAG, "Start the Chaser animation");

    while (1) {
        chaserAnimation();
        vTaskDelay(10 / portTICK_PERIOD_MS); // delay for visibility
    }
}
