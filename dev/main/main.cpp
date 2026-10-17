/*
*******************************************************************************
    Test Program, to showcase the major functions of the Neopixel Driver
    Uses animation functions in separate *.cpp files.

    The program also monitors and logs the statistics of the Neopixel Driver.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"

#include "config_gpio.h"      // automatic selection of GPIO pins, based on slelected Target Device
#include "config_neopixels.h" // manually set Neopixel type and number of Neopixels here

static const char *TAG = "MAIN";

//-----------------------------------------------
//  Prototypes of external animation functions
//  (not using a *.h only for this)
//-----------------------------------------------
void logStatistics(void);
bool animateRanges(unsigned long currentMillis);
bool animatePause(unsigned long currentMillis);

/*
===============================================================================
    Start the Neopixel Driver
===============================================================================
*/
MyPixelDriver npx; // global variable, to be used by animations

static void startNeopixel(void) {
    //-------------------------------------------
    //  Check on nrPixels
    //-------------------------------------------
    if (nrNeopixels < 12) {
        ESP_LOGE(TAG, "STOPPED:At least 12 Neopixels are required to run this program, configured nrNeopixels=%d", nrNeopixels);
        for (;;) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    //-------------------------------------------
    //  Optional: set Enable output to High
    //-------------------------------------------
    if (GPIO_IS_VALID_OUTPUT_GPIO(enablePin)) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured (pin = %d)", enablePin);
    }

    //-------------------------------------------
    //  Start the Neopixel Driver
    //-------------------------------------------
    if (!npx.begin(nrNeopixels, dataPin)) {
        ESP_LOGE(TAG, "STOPPED: Failed to start the Neopixel Driver");
        for (;;) {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

    //-------------------------------------------
    //  Preset the Neopixels
    //-------------------------------------------
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixel ring
    npx.brightness = 0x10;           // medium brightness
}

/*
===============================================================================
    Classic on/off Status LED as heartbeat
===============================================================================
*/
static void configureStatusLed(void) {
    if (GPIO_IS_VALID_OUTPUT_GPIO(statusLedPin)) {
        ESP_LOGI(TAG, "Status LED configured on pin=%d", statusLedPin);
        gpio_reset_pin(statusLedPin);
        gpio_set_direction(statusLedPin, GPIO_MODE_OUTPUT);
    } else {
        ESP_LOGI(TAG, "Optional Status LED is NOT configured (pin = %d)", statusLedPin);
    }
}

static void toggleStatusLed(void) {
    if (GPIO_IS_VALID_OUTPUT_GPIO(statusLedPin)) {
        static bool isOn = false;
        gpio_set_level(statusLedPin, isOn);
        isOn = !isOn;
    }
}

/*
*******************************************************************************
    Main
*******************************************************************************
*/
extern "C" void app_main(void) {
    // As desired, increase logging for the Neopixel Driver, the I2S subsystem and the animations
    // Do NOT set it for all components ("*") because that would show each I2S enable/disable at ESP-IDF level
    esp_log_level_set(TAG, ESP_LOG_DEBUG);
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);
    // esp_log_level_set("RANGES", ESP_LOG_DEBUG);
    // esp_log_level_set("PAUSE", ESP_LOG_DEBUG);
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect

    ESP_LOGI(TAG, "----- Starting Test Program, chip=`%s` -----", CONFIG_IDF_TARGET);

    configureStatusLed();
    startNeopixel();

    unsigned long currentMillis = esp_timer_get_time() / 1000;
    unsigned long toggleTimeoutMillis = currentMillis;

    while (1) {
        static int animationMode = 0;
        currentMillis = esp_timer_get_time() / 1000;

        // Status LED animation (heartbeat)
        if ((long)(currentMillis - toggleTimeoutMillis) >= 0) {
            toggleStatusLed();
            toggleTimeoutMillis = currentMillis + 100;
        }

        // Check and log statistics on the Neopixel Driver
        logStatistics();

        // Neopixel animations
        switch (animationMode) {
        case 0:
            if (animateRanges(currentMillis)) {
                animationMode++;
            }
            break;
        case 1:
            if (animatePause(currentMillis)) {
                animationMode++;
            }
            break;
        default:
            animationMode = 0; // reset to the first animation mode
            break;
        }
        vTaskDelay(1); // to prevent watchdog reset in case of animation without any delay
    }
}
