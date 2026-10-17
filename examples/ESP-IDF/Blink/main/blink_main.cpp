/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on ESP-IDF: Blink animation

    Blink both the Status LED (on standard GPIO) and one (1) Neopixel at the same time.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

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

    bool isOn = true;

    ESP_LOGI(TAG, "Start the Blink animation");

    while (1) {
        if (isOn) {
            npx.setPixel(0, neopixelRed);
        } else {
            npx.setPixel(0, neopixelBlack);
        }
        npx.show();

        isOn = !isOn;                         // toggle for next iteration
        vTaskDelay(100 / portTICK_PERIOD_MS); // for visibility, wait [ms] before the next iteration
    }
}
