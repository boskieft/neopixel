/*
***************************************************************************************************
    Example of using the ESP32xx Neopixel Driver on pioarduino: Range animation

    Demonstrate filling a pixel range in one half (2 quadrants)
    and running the Chaser (using rotate) in the other half.

    After each Chase sequence (3 rounds), the quadrants shift one step
    (90 degrees in case of a ring), to demonstrate the correct wrap around behavior
    of the Driver in the SetPixelRange and rotateRangeLeft/Right functions.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <Arduino.h>
#include "neopixel.h"

static const char *TAG = "RANGE"; // for ESP_LOGx() logging

/*
===============================================================================
    Neopixel Configuration, adjust as needed for your own hardware setup
===============================================================================
*/
static const gpio_num_t dataPin = GPIO_NUM_5; // output data pin to DI of the Neopixels
static const size_t nrNeopixels = 24;         // nr of Neopixels to drive (at least 12 for a nice animation)
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
    ESP_LOGI(TAG, "Start the Range animation");

    if (!npx.isRotatable()) {
        ESP_LOGW(TAG, "This `%s`/PixelType combination does NOT support the rotate method, so cannot do Chaser animation", CONFIG_IDF_TARGET);
        return;
    }
}

/*
*******************************************************************************
    Run Chaser with 3 pixels in the specified range,
    rotate a few times Right and then back Left.
*******************************************************************************
*/
static void chaserInRange(size_t startIndex, size_t endIndex) {
    static const size_t nrRotations = 3 * (nrNeopixels / 2) - 3; // 3 full rotations, end with the 3 pixels back in start position

    static const int delayMillis = 200; // for visibility

    // Set 3 pixels at the start position
    npx.setPixel(startIndex, neopixelBlue);
    npx.setPixel(startIndex + 1, neopixelGreen);
    npx.setPixel(startIndex + 2, neopixelRed); // on start this is the front pixel
    npx.show();

    if (!npx.isRotatable()) {
        return; // cannot do Chaser with rotate
    }

    for (int i = 0; i < 1; i++) {
        // Rotate Right (i.e. in the direction of increasing pixel indices)
        for (int j = 0; j < nrRotations; j++) {
            delay(delayMillis);
            npx.rotateRangeRight(startIndex, endIndex);
            npx.show();
        }
        // Rotate Left
        for (int j = 0; j < nrRotations; j++) {
            delay(delayMillis);
            npx.rotateRangeLeft(startIndex, endIndex);
            npx.show();
        }
    }
    delay(delayMillis);
}

/*
*******************************************************************************
    Main

    The Neopixels are divided into four quarters,
    that are shifted (ring: rotated) after each Chaser sequence.
*******************************************************************************
 */
void loop(void) {
    static const size_t startIndexQ1 = 0;
    static const size_t startIndexQ2 = (1 * nrNeopixels) / 4;
    static const size_t startIndexQ3 = (2 * nrNeopixels) / 4;
    static const size_t startIndexQ4 = (3 * nrNeopixels) / 4;

    static PixelColor dimmedWhite = makePixelColor(0x20, 0x20, 0x20);
    static const int delayMillis = 1000; // for better UX

    //---------------------------------
    //  Q1+Q2 white, Chaser in Q3+Q4
    //---------------------------------
    npx.setPixelRange(startIndexQ3, nrNeopixels - 1, neopixelBlack);
    npx.setPixelRange(startIndexQ1, startIndexQ3 - 1, dimmedWhite);
    npx.show();
    delay(delayMillis);                           // delay to show the white range before starting the chaser
    chaserInRange(startIndexQ3, nrNeopixels - 1); // run the Chaser animation
    delay(delayMillis);                           // delay to show the end position of the Chaser pixels

    //---------------------------------
    //  Q2+Q3 white, Chaser in Q4+Q1
    //---------------------------------
    npx.setPixelRange(startIndexQ4, startIndexQ2 - 1, neopixelBlack); // note that this wraps around the end of the Neopixels
    npx.setPixelRange(startIndexQ2, startIndexQ4 - 1, dimmedWhite);
    npx.show();
    delay(delayMillis);
    chaserInRange(startIndexQ4, startIndexQ2 - 1); // note that this wraps around the end of the Neopixels
    delay(delayMillis);

    //---------------------------------
    //  Q3+Q4 white, Chaser in Q1+Q2
    //---------------------------------
    npx.setPixelRange(startIndexQ1, startIndexQ3 - 1, neopixelBlack);
    npx.setPixelRange(startIndexQ3, nrNeopixels - 1, dimmedWhite);
    npx.show();
    delay(delayMillis);
    chaserInRange(startIndexQ1, startIndexQ3 - 1);
    delay(delayMillis);

    //---------------------------------
    //  Q4+Q1 white, Chaser in Q2+Q3
    //---------------------------------
    npx.setPixelRange(startIndexQ2, startIndexQ4 - 1, neopixelBlack);
    npx.setPixelRange(startIndexQ4, startIndexQ2 - 1, dimmedWhite); // note that this wraps around the end of the Neopixels
    npx.show();
    delay(delayMillis);
    chaserInRange(startIndexQ2, startIndexQ4 - 1);
    delay(delayMillis);
}
