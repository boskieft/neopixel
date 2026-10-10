/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on ESP-IDF: Chaser animation

    The Neopixels will be lit one after each other

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "neopixel.h"

static const char *TAG = "CHASER"; // for ESP_LOGx() logging

/*
-----------------------------------------------------------
    GPIO pins -> Adapt to your hardware configuration !
-----------------------------------------------------------
*/
// My ESP32-C3 config
static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
static const size_t nrNeopixels = 24;            // nr of Neopixels to drive

NeopixelDriver<PixelType::GRB_SEQ3> npx;

void startNeopixel(void) {
    // Optional: Enable the level shifter
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }

    // Optional: Increase logging of the Neopixel driver and its I2S subsystem
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);

    // Start the Neopixel driver
    ESP_LOGI(TAG, "Starting the Neopixel driver on pin=%d with %d pixels", dataPin, nrNeopixels);
    npx.begin(nrNeopixels, dataPin);
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
}

void chaserAnimation(void) {
    static int coloredIndex = 0;
    static int blackIndex = nrNeopixels - 1;

    npx.setPixel(blackIndex, neopixelBlack); // erase previously colored pixel
    npx.setPixel(coloredIndex, neopixelRed); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= nrNeopixels) {
        coloredIndex = 0; // new loop
    }
}

/*
***********************************************************
    Main
***********************************************************
 */
extern "C" void app_main(void) {
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect
    startNeopixel();

    ESP_LOGI(TAG, "Start the Chaser animation");

    while (1) {
        chaserAnimation();
        vTaskDelay(10 / portTICK_PERIOD_MS); // delay for visibility
    }
}
