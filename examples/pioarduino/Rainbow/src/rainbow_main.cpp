/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on pioarduino: Rainbow animation

    Rainbow colors will be shifted (strip) / rotated (ring) across the Neopixels.
    Two methods are demonstrated: recalculating each frame and just rotating the pixels.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "neopixel.h"

static const char *TAG = "RAINBOW"; // for ESP_LOGx() logging

/*
===============================================================================
    Neopixel Configuration, adjust as needed for your own hardware setup
===============================================================================
*/
static const gpio_num_t dataPin = GPIO_NUM_5; // output data pin to DI of the Neopixels
static const size_t nrNeopixels = 24;         // nr of Neopixels to drive
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
// Set the delay times in such way that the animation speed of both methods is the same
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
            npx.rotateAllLeft();
            npx.show();
            delay(ROTATE_METHOD_DELAY_MS); // delay for visibility
        }
    } else {
        ESP_LOGW(TAG, "SKIP: This `%s`/PixelType combination does NOT support the rotate method", CONFIG_IDF_TARGET);
    }
}
