/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "esp_log.h"

#include "neopixel.h"

#define TAG "MAIN"

//-----------------
//  3x RGB ring
//-----------------
NeopixelDriver<PixelType::GRB_SEQ3> npx;
inline constexpr PixelColor neopixelColored = neopixelRed;
inline constexpr PixelColor neopixelBackgroundColor = {.color = {.b = 0x10, .g = 0, .r = 0, .w = 0}}; // dimmed Blue

#define PIXEL_COUNT (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32) // 1 ring of 60, 1 ring of 24, 1 assembly of 6 rings
// #define PIXEL_COUNT (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32 - 1) // test: 1 pixel less
//  #define PIXEL_COUNT (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32 + 1) // test: 1 pixel more
//  #define PIXEL_COUNT 61

/*
-------------------------------------------------------------------------------
    Set the GPIO pins based on the target chip selected when building

    Should match your actual hardware (wiring) configuration, adapt as needed.
-------------------------------------------------------------------------------
*/
gpio_num_t dataPin = GPIO_NUM_NC;

bool setGPIO(void) {
    gpio_num_t enablePin = GPIO_NUM_NC;
    if (strcmp(CONFIG_IDF_TARGET, "esp32") == 0) {
        dataPin = GPIO_NUM_19;
        enablePin = GPIO_NUM_26;
    } else if (strcmp(CONFIG_IDF_TARGET, "esp32s2") == 0) {
        dataPin = GPIO_NUM_9;
        enablePin = GPIO_NUM_5;
    } else if (strcmp(CONFIG_IDF_TARGET, "esp32s3") == 0) {
        dataPin = GPIO_NUM_17;
        enablePin = GPIO_NUM_16;
    } else if (strcmp(CONFIG_IDF_TARGET, "esp32c3") == 0) {
        dataPin = GPIO_NUM_5;
        enablePin = GPIO_NUM_10;
    } else if (strcmp(CONFIG_IDF_TARGET, "esp32c6") == 0) {
        dataPin = GPIO_NUM_2;
        enablePin = GPIO_NUM_21;
    }

    if (dataPin == GPIO_NUM_NC) {
        ESP_LOGE(TAG, "No dataPin configured for Chip=%s", CONFIG_IDF_TARGET);
        return (false);
    }

    ESP_LOGI(TAG, "Chip=`%s`, using dataPin=%d", CONFIG_IDF_TARGET, dataPin);
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }
    return true;
}

/*
-------------------------------------------------------------------------------
    Start the Neopixel driver
-------------------------------------------------------------------------------
*/
bool startNeopixelDriver(void) {
    if (!setGPIO()) {
        return (false);
    }

    ESP_LOGI(TAG, "Init the Neopixels on pin=%d with %d pixels", dataPin, PIXEL_COUNT);
    npx.begin(PIXEL_COUNT, dataPin);
    //@@@TODO: error handling

    if (npx.isRotatable()) {
        ESP_LOGI(TAG, "Neopixels are rotatable");
    } else {
        ESP_LOGI(TAG, "Neopixels are NOT rotatable");
    }

    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
    return (true);
}

/*
-------------------------------------------------------------------------------
    Show (log) the memory info
-------------------------------------------------------------------------------
*/
void showMemoryInfo(void) {
    ESP_LOGI(TAG, "---Memory---");
    ESP_LOGI(TAG, "Free heap=%" PRIu32, esp_get_free_internal_heap_size());
    ESP_LOGI(TAG, "Largest free block=%" PRIu32, heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL));
    ESP_LOGI(TAG, "Min free heap=%" PRIu32, esp_get_minimum_free_heap_size());
}

/*
-------------------------------------------------------------------------------
    Animate a moving pixel on the Neopixels
-------------------------------------------------------------------------------
*/
void movingPixel(void) {
    static int coloredIndex = 0;
    static int blackIndex = PIXEL_COUNT - 1;

    npx.setPixel(blackIndex, neopixelBlack);     // erase previously colored pixel
    npx.setPixel(coloredIndex, neopixelColored); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= PIXEL_COUNT) {
        // New loop
        coloredIndex = 0;
    }
}

/*
===============================================================================
    Main
===============================================================================
*/
extern "C" void app_main(void) {
    vTaskDelay(5000 / portTICK_PERIOD_MS); // allow terminal to initialize

    printf("------------------------------\n");
    esp_log_level_set("*", ESP_LOG_DEBUG); // Set log level to include ESP_LOGD messages
    ESP_LOGI(TAG, "Starting application...");
    showMemoryInfo();
    startNeopixelDriver();
    showMemoryInfo();

    for (;;) {
        for (int i = 0; i < 3 * PIXEL_COUNT; i++) {
            movingPixel();
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds between each pixel movement

        for (int i = 0; i < 20; i++) {
            npx.rotateLeft();
            npx.show();                           // send the rotated data to the Neopixels
            vTaskDelay(100 / portTICK_PERIOD_MS); // delay for 100 milliseconds between each rotation
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds between each pixel movement

        for (int i = 0; i < 20; i++) {
            npx.rotateRight();
            npx.show();                           // send the rotated data to the Neopixels
            vTaskDelay(100 / portTICK_PERIOD_MS); // delay for 100 milliseconds between each rotation
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds between each pixel movement

        npx.setAllPixels(neopixelBackgroundColor);
        npx.show();
        showMemoryInfo();

        vTaskDelay(1000 / portTICK_PERIOD_MS); // delay for 1000 milliseconds between each pixel movement
    }
    printf("Restarting in 10 seconds...\n");
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
