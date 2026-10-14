/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on ESP-IDF: Blink animation

    Blink both the Status LED (on standard GPIO) and one (1) Neopixel at the same time.

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

static const char *TAG = "BLINK"; // for ESP_LOGx() logging

/*
-----------------------------------------------------------
    GPIO pins -> Adapt to your hardware configuration !
-----------------------------------------------------------
*/
// My ESP32-C3 config
static const gpio_num_t statusLedPin = GPIO_NUM_8; // output pin to drive the classic on/off Status LED

static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of the Neopixels (via 74HCT126 level shifter)

/*
-----------------------------------------------------------
    Classic on/off Status LED
-----------------------------------------------------------
*/
static void configureStatusLed(void) {

    if (statusLedPin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Status LED on GPIO=%d", statusLedPin);
        gpio_reset_pin(statusLedPin);
        gpio_set_direction(statusLedPin, GPIO_MODE_OUTPUT);
    } else {
        ESP_LOGW(TAG, "Status LED pin is NOT configured (pin = %d), Status LED will not blink", statusLedPin);
    }
}

static void blinkStatusLed(bool isOn) {
    if (statusLedPin != GPIO_NUM_NC) {
        gpio_set_level(statusLedPin, !isOn); // LED is active low
    }
}

/*
-----------------------------------------------------------
    Neopixel LED
-----------------------------------------------------------
*/
NeopixelDriver<PixelType::GRB_SEQ3> npx;
static const size_t nrNeopixels = 1; // just one (1) pixel for this example

void startNeopixel(void) {
    // Optional: Enable the level shifter
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured (pin = %d)", enablePin);
    }

    // Start the Neopixel Driver
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

void blinkNeopixel(bool isOn) {
    if (isOn) {
        npx.setPixel(0, neopixelRed);
    } else {
        npx.setPixel(0, neopixelBlack);
    }
    npx.show();
}

/*
***********************************************************
    Main
***********************************************************
 */
extern "C" void app_main(void) {
    // Increase logging for this program, the Neopixel Driver and the I2S subsystem.
    // Do NOT set it for all components ("*") because that would show each I2S enable/disable at ESP-IDF level
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect

    ESP_LOGI(TAG, "----- Startup, chip=`%s` -----", CONFIG_IDF_TARGET);
    configureStatusLed();
    startNeopixel();

    bool isOn = true;

    ESP_LOGI(TAG, "Start the Blink animation");

    while (1) {
        blinkStatusLed(isOn);
        blinkNeopixel(isOn);

        /* Toggle the LED state */
        isOn = !isOn;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
