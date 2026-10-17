/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on ESP-IDF: Chaser animation

    The Neopixels will be lit one after another.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "neopixel.h"

static const char *TAG = "CHASER"; // for ESP_LOGx() logging

/*
===============================================================================
    Neopixel Configuration, adjust as needed for your own hardware setup
===============================================================================
*/
static const gpio_num_t dataPin = GPIO_NUM_5; // output data pin to DI of the Neopixels
static const size_t nrNeopixels = 24;         // number of Neopixels to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;      // 3 colors (no white) in the usual GRB order, SEQ3 timing

/*
-------------------------------------------------------------------------------
    Start the Neopixel Driver
-------------------------------------------------------------------------------
*/
void startNeopixel(void) {
    if (!npx.begin(nrNeopixels, dataPin)) {
        ESP_LOGE(TAG, "STOPPED: Failed to start the Neopixel Driver");
        for (;;) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    // Preset the Neopixels
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
}

/*
-------------------------------------------------------------------------------
    Chaser animation

    Lights one Neopixel at a time in a chasing sequence.
-------------------------------------------------------------------------------
*/
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
*******************************************************************************
    Main
*******************************************************************************
 */
extern "C" void app_main(void) {
    // Increase logging for this program, the Neopixel Driver and the I2S subsystem.
    // Do NOT set it for all components ("*") because that would show each I2S enable/disable at ESP-IDF level
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect

    ESP_LOGI(TAG, "----- Startup, chip=`%s` -----", CONFIG_IDF_TARGET);
    startNeopixel();

    ESP_LOGI(TAG, "Start the Chaser animation");

    while (1) {
        chaserAnimation();
        vTaskDelay(10 / portTICK_PERIOD_MS); // delay for visibility
    }
}
