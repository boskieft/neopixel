/*
*******************************************************************************
    Ranges test function

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "config_neopixels.h"

static const char *TAG = "RANGES";

/*
-------------------------------------------------------------------------------
    Run the Chaser in a specified range of Neopixels

    Returns true when animation is complete
-------------------------------------------------------------------------------
*/
static bool chaserInRange(unsigned long currentMillis, size_t startIndex, size_t endIndex) {
    static const size_t nrRotations = 3 * (nrNeopixels / 2) - 3; // 3 full rotations, end with the 3 pixels back in start position
    static int chaseStatus = 0;
    static int rotationCounter = 0;
    static bool isBackward = 0;
    static unsigned long timeoutMillis = 0;

    switch (chaseStatus) {
    case 0:
        //---------------------------------
        //  Set 3 pixels at start position
        //---------------------------------
        npx.setPixel(startIndex, neopixelBlue);
        npx.setPixel(startIndex + 1, neopixelGreen);
        npx.setPixel(startIndex + 2, neopixelRed); // on start this is the front pixel
        npx.show();
        isBackward = false; // start by moving to the right
        chaseStatus++;
        timeoutMillis = currentMillis + 1000; // delay for visibility of start position
        ESP_LOGD(TAG, "Chaser animation started at startIndex=%d", startIndex);
        break;

    case 1:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        if (!npx.isRotatable()) {
            ESP_LOGW(TAG, "Chaser animation skipped, not rotatable");
            chaseStatus = 0;
            return (true); // done without Chaser
        }

        //---------------------------------
        //  Let's go
        //---------------------------------
        isBackward = false; // start by rotating forward (to the Right)
        rotationCounter = 0;
        chaseStatus++;
        ESP_LOGD(TAG, "Chaser let's go");
        break;

    case 2:
        //---------------------------------
        //  Rotate
        //---------------------------------
        if (isBackward) {
            npx.rotateRangeLeft(startIndex, endIndex);
        } else {
            npx.rotateRangeRight(startIndex, endIndex);
        }
        npx.show();
        timeoutMillis = currentMillis + 100; // delay between rotation steps
        chaseStatus++;
        break;

    case 3:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        //  Next rotation step
        //---------------------------------
        if (++rotationCounter < nrRotations) {
            chaseStatus = 2; // continue in this direction
            return (false);  // rotating
        }

        if (isBackward) {
            // Reached the end of the backward rotation, done
            chaseStatus = 0;
            ESP_LOGD(TAG, "Chaser animation done");
            return (true); // done with Chaser
        }

        // Reached the end of the forward rotation
        isBackward = true; // change direction to backward
        chaseStatus = 2;   // go back to rotating
        rotationCounter = 0;
        ESP_LOGD(TAG, "Chaser go backward");
        break;

    default:
        ESP_LOGE(TAG, "Unexpected chaseStatus=%d", chaseStatus);
        chaseStatus = 0;
        return (true); // done with error
        break;
    }

    return (false); // not done yet
}

/*
*******************************************************************************
    Ranges animation

    Returns true when animation is complete
*******************************************************************************
*/
bool animateRanges(unsigned long currentMillis) {
    static const size_t startIndexQ1 = 0;
    static const size_t startIndexQ2 = (1 * nrNeopixels) / 4;
    static const size_t startIndexQ3 = (2 * nrNeopixels) / 4;
    static const size_t startIndexQ4 = (3 * nrNeopixels) / 4;

    static PixelColor dimmedWhite = MakePixelColor(0x20, 0x20, 0x20);
    static int animationStatus = 0;
    static unsigned long timeoutMillis = 0;

    struct rangeIndexes {
        size_t startChaser;
        size_t endChaser;
        size_t startKeepout;
        size_t endKeepout;
    };

    static struct rangeIndexes quadrantIndexes[4] = {
        {startIndexQ1, startIndexQ3 - 1, startIndexQ3, nrNeopixels - 1},
        {startIndexQ2, startIndexQ4 - 1, startIndexQ4, startIndexQ2 - 1},
        {startIndexQ3, nrNeopixels - 1, startIndexQ1, startIndexQ3 - 1},
        {startIndexQ4, startIndexQ2 - 1, startIndexQ2, startIndexQ4 - 1}};

    static int quadrant = 0; //@@@TODO: rename?

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Set the pixels for the keepout
        //  and chaser ranges
        //---------------------------------
        npx.setPixelRange(quadrantIndexes[quadrant].startKeepout, quadrantIndexes[quadrant].endKeepout, dimmedWhite);
        npx.setPixelRange(quadrantIndexes[quadrant].startChaser, quadrantIndexes[quadrant].endChaser, neopixelBlack);
        npx.show();
        animationStatus++;
        timeoutMillis = currentMillis + 1000; // delay for visibility of keepout area only
        ESP_LOGD(TAG, "(Re)Starting animation, quadrant=%d", quadrant);
        break;

    case 1:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        // Let's go
        //---------------------------------
        animationStatus++;
        break;

    case 2:
        //---------------------------------
        //  Run the Chaser animation
        //  in this quadrant
        //---------------------------------
        if (chaserInRange(currentMillis, quadrantIndexes[quadrant].startChaser, quadrantIndexes[quadrant].endChaser)) { // run the Chaser animation
            // Chaser is done for this quadrant
            animationStatus++;
            timeoutMillis = currentMillis + 1000; // delay for visibility of end position
        } // else: keep the chaser running
        break;

    case 3:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        //  Quadrant done
        //---------------------------------
        if (++quadrant > 3) {
            // All quadrant done
            ESP_LOGD(TAG, "All quadrants done");
            animationStatus = 0;
            quadrant = 0;
            return (true); // indicate that animation is done
        } else {
            // Next quadrant
            ESP_LOGD(TAG, "Next quadrant");
            animationStatus = 0;
        }
        break;

    default:
        ESP_LOGE(TAG, "Unexpected animationStatus=%d", animationStatus);
        animationStatus = 0;
        return (true); // done with error
        break;
    }
    return (false); // not done yet
}
