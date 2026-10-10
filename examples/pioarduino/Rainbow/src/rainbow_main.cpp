/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on Pioarduino: Rainbow animation

    Rainbow colors will be shifted (strip) / rotated (ring) across the Neopixels.
    Two methods are demonstrated: recalculating each frame and just rotating the pixels.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "set_gpio.h" // comment-out to define the GPIO pins manually below
#include "neopixel.h"

static const char *TAG = "RAINBOW"; // for ESP_LOGx() logging

#ifndef SET_GPIO_H
// instead of using "set_gpio.h" you can also set the GPIO pins manually here
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of the Neopixels (via 74HCT126 level shifter)
static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
#endif

/*
===============================================================================
    Start the Neopixel driver
===============================================================================
*/
static const size_t nrNeopixels = 24; // nr of Neopixels to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;

void setup(void) {
    // Logging
    esp_log_level_set("*", ESP_LOG_DEBUG); // set log level to include ESP_LOGD messages
    delay(3000);                           // wait for Platformio monitor to connect

    ESP_LOGI(TAG, "----- Running setup, chip=`%s` -----", CONFIG_IDF_TARGET);

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
    ESP_LOGI(TAG, "Start the Rainbow animation");
}

/*
===============================================================================
    Rainbow effect
===============================================================================
 */
//---------------------------------------------------------
//  Convert hue value to PixelColor
//---------------------------------------------------------
PixelColor hueToPixelColor(
    uint8_t hue) { // hue color value [0...255], 0=red, goes via orange, yellow... to magenta and back to red

    PixelColor pixel;
    hue = 255 - hue; // invert to [255...0] for easy calculation here

    if (hue < 85) {
        pixel.color.r = static_cast<uint8_t>(255 - hue * 3);
        pixel.color.g = 0;
        pixel.color.b = static_cast<uint8_t>(hue * 3);
        return pixel;
    }

    if (hue < 170) {
        hue -= 85;
        pixel.color.r = 0;
        pixel.color.g = static_cast<uint8_t>(hue * 3);
        pixel.color.b = static_cast<uint8_t>(255 - hue * 3);
        return pixel;
    }

    hue -= 170;
    pixel.color.r = static_cast<uint8_t>(hue * 3);
    pixel.color.g = static_cast<uint8_t>(255 - hue * 3);
    pixel.color.b = 0;
    return pixel;
}

//---------------------------------------------------------
//  Set rainbow colors with an offset to the Neopixels
//---------------------------------------------------------
void rainbow(
    uint8_t offset) { // Offset [0...255] for the rainbow distribution, 0 means Neopixel 0 is red
    uint8_t hue;
    for (size_t i = 0; i < nrNeopixels; ++i) {
        hue = static_cast<uint8_t>((offset + (i * 256) / nrNeopixels) & 0xff); // colors distributed across the Neopixels, shifted by offset
        npx.setPixel(i, hueToPixelColor(hue));
    }
    npx.show(); // send the updated rainbow colors to the Neopixels
}

/*
*******************************************************************************
    Main
*******************************************************************************
 */
#define RECALCULATE_METHOD_DELAY_MS (50)
#define ROTATE_METHOD_DELAY_MS ((RECALCULATE_METHOD_DELAY_MS * 256) / nrNeopixels) // Use same pixel speed for the rotating method

void loop(void) {
    ESP_LOGI(TAG, "Method: recalculate each frame");
    // Smooth effect, regardless of nr of Neopixels (even just one)
    for (size_t i = 0; i < 256; i++) {
        rainbow(static_cast<uint8_t>(i));
        delay(RECALCULATE_METHOD_DELAY_MS); // delay for visibility
    }

    if (npx.isRotatable()) {
        ESP_LOGI(TAG, "Method: just rotate pixels");
        // Effect is still a bit jumpy with 24 Neopixels, fine with 60
        for (size_t i = 0; i < nrNeopixels; i++) {
            npx.rotateLeft();
            npx.show();
            delay(ROTATE_METHOD_DELAY_MS); // delay for visibility
        }
    } else {
        ESP_LOGW(TAG, "SKIP: This `%s`/PixelType combination does NOT support the rotate method", CONFIG_IDF_TARGET);
    }
}
