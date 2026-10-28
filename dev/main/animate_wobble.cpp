/*
*******************************************************************************
    Wobble test function

    Move the Neopixels left and right

    Precondition is that initial position of the Neopixels is set

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "config_neopixels.h"

static const char *TAG = "WOBBLE";

/*
-------------------------------------------------------------------------------
    Wobble the Neopixels one (1) cycle left, right and back left

    Returns true when done
-------------------------------------------------------------------------------
*/
static bool wobble(unsigned long currentMillis) {
    static const int maxAmplitude = 3; // number of pixels to wobble left and right, relative to initial position
    static int currentAmplitude = 0;

    static const int wobbleStepDelay = 100;  // delay for each wobble step in milliseconds
    static const int endPositionDelay = 200; // delay for visibility of end positions

    static int wobbleStatus = 0;
    static unsigned long timeoutMillis = 0;

    if ((long)(currentMillis - timeoutMillis) < 0) {
        return (false); // waiting
    }

    switch (wobbleStatus) {
    case 0:
        //---------------------------------
        //  Initialize wobble
        //---------------------------------
        currentAmplitude = 0;
        wobbleStatus++;
        break;

    case 1:
        //---------------------------------
        //  Move right
        //---------------------------------
        npx.rotateAllRight();
        npx.show();
        currentAmplitude++;

        if (currentAmplitude >= maxAmplitude) {
            // Full right
            timeoutMillis = currentMillis + endPositionDelay; // delay for visibility of end Right position
            wobbleStatus++;
            break;
        }
        timeoutMillis = currentMillis + wobbleStepDelay; // delay for visibility of wobbling
        break;

    case 2:
        //---------------------------------
        //  Move left
        //---------------------------------
        npx.rotateAllLeft();
        npx.show();
        currentAmplitude--;

        if (currentAmplitude <= (0 - maxAmplitude)) {
            // Full left
            timeoutMillis = currentMillis + endPositionDelay; // delay for visibility of end Left position
            wobbleStatus++;
            break;
        }
        timeoutMillis = currentMillis + wobbleStepDelay; // delay for visibility of wobbling
        break;

    case 3:
        //---------------------------------
        //  Move right to initial position
        //---------------------------------
        npx.rotateAllRight();
        npx.show();
        currentAmplitude++;

        if (currentAmplitude >= 0) {
            // At initial position again
            timeoutMillis = currentMillis + wobbleStepDelay; // delay for visibility of this last step
            wobbleStatus++;
            break;
        }
        timeoutMillis = currentMillis + wobbleStepDelay; // delay for visibility of wobbling
        break;

    case 4:
        //---------------------------------
        //  Finalising
        //---------------------------------
        wobbleStatus = 0;
        return (true); // done with wobbling
        break;

    default:
        //---------------------------------
        // Unexpected status
        //---------------------------------
        ESP_LOGE(TAG, "Unexpected wobbleStatus=%d", wobbleStatus);
        wobbleStatus = 0;
        return (true); // done with error
        break;
    }
    return false; // not done yet
}

/*
*******************************************************************************
    Wobble animation

    Returns true when animation is complete
*******************************************************************************
*/
bool animateWobble(unsigned long currentMillis) {
    static const int nrWobbles = 3; // nr of wobble cycles
    static int wobbleCounter = 0;

    static int animationStatus = 0;
    static unsigned long timeoutMillis = 0;

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Initialize animation
        //---------------------------------
        if (!npx.isRotatable()) {
            ESP_LOGW(TAG, "Wobble animation skipped, not rotatable");
            return (true); // cannot perform wobble animation
        }

        ESP_LOGI(TAG, "Start Wobble animation with the currently lit Neopixels");
        animationStatus++;
        timeoutMillis = currentMillis + 500; // delay for initial position
        break;

    case 1:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        // Let's go
        //---------------------------------
        animationStatus++;
        wobbleCounter = 1;
        break;

    case 2:
        //---------------------------------
        //  Run the Wobble animation
        //---------------------------------
        if (wobble(currentMillis)) {
            if (wobbleCounter >= nrWobbles) {
                wobbleCounter = 0;
                animationStatus++;
                timeoutMillis = currentMillis + 500; // delay for final position
                break;
            }
            wobbleCounter++;
        } // else: keep wobbling
        break;

    case 3:
        //---------------------------------
        //  Finalising
        //---------------------------------
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        animationStatus = 0;
        return (true); // indicate that animation is done
        break;

    default:
        //---------------------------------
        // Unexpected status
        //---------------------------------
        ESP_LOGE(TAG, "Unexpected Wobble animationStatus=%d", animationStatus);
        animationStatus = 0;
        return (true); // done with error
        break;
    }
    return (false); // not done yet
}
