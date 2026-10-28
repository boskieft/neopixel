/*
*******************************************************************************
    Brightness test function

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "config_neopixels.h"

static const char *TAG = "BRIGHT";

/*
-------------------------------------------------------------------------------
    Perform simple gamma correction on the raw brightness value

    This way, the perceived brightness changes more smoothly and naturally.
-------------------------------------------------------------------------------
*/
static uint8_t gammaCorrection(uint8_t raw) {
    uint32_t squared = ((uint32_t)raw * raw) >> 8;
    uint32_t result = (3 * squared + ((squared * squared) >> 8)) / 4;
    if (raw > 0 && result < 2) {
        result = 2;
    }
    return (uint8_t)result;
}

/*
-------------------------------------------------------------------------------
    Fade brightness up or down for specified Neopixels

    Returns true when reached the target brigh999tness
-------------------------------------------------------------------------------
*/
static bool fadeBrightness(unsigned long currentMillis, bool isUp) {
    static const size_t pixelIndexArray[] = {
        0,
        (1 * nrNeopixels) / 4,
        (2 * nrNeopixels) / 4,
        (3 * nrNeopixels) / 4};
    static const PixelColor pixelColorArray[] = {
        neopixelRed,
        neopixelGreen,
        neopixelBlue,
        npx.hasWhite() ? neopixelWhite_RGBW : neopixelWhite_RGB};

    static const int minBrightness = 1; // Neopixels will not lit below this value
    // static const int minBrightness = 2; // Neopixels will not lit below this value
    // static const int maxBrightness = 63; // Neopixels will be too bright above this value
    static const int maxBrightness = 127; // Neopixels will be too bright above this value

    static int currentBrightness = 0; // int, to allow detecting overflow during increase/decrease

    static int fadeStatus = 0;
    static unsigned long timeoutMillis = 0;

    switch (fadeStatus) {
    case 0:
        //---------------------------------
        //  Init
        //---------------------------------
        if (isUp) {
            currentBrightness = minBrightness;
        } else {
            currentBrightness = maxBrightness;
        }
        timeoutMillis = currentMillis + 1000; // delay for visibility of initial brightness
        fadeStatus++;
        break;

    case 1:
        //---------------------------------
        //  Show pixels
        //---------------------------------
        // npx.brightness = static_cast<uint8_t>(currentBrightness);
        // npx.brightness = applyGammaCorrection(static_cast<uint8_t>(currentBrightness), 2.2f);
        npx.brightness = gammaCorrection(static_cast<uint8_t>(currentBrightness));
        for (size_t i = 0; i < (sizeof(pixelIndexArray) / sizeof(pixelIndexArray[0])); i++) {
            npx.setPixel(pixelIndexArray[i], pixelColorArray[i]);
        }
        npx.show();
        fadeStatus++;
        break;

    case 2:
        //---------------------------------
        //  Wait and next fading step
        //---------------------------------
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        if (isUp) {
            // Fading up
            if (currentBrightness >= maxBrightness) {
                fadeStatus = 0;
                return (true); // reached max brightness, done
            } else {
                currentBrightness++;
            }
        } else {
            // Fading down
            if (currentBrightness <= minBrightness) {
                fadeStatus = 0;
                return (true); // reached min brightness, done
            } else {
                currentBrightness--;
            }
        }
        fadeStatus = 1;
        timeoutMillis = currentMillis + 20; // delay for visibility of brightness change
        break;

    default:
        //---------------------------------
        // Unexpected status
        //---------------------------------
        ESP_LOGE(TAG, "Unexpected fadeStatus=%d", fadeStatus);
        fadeStatus = 0;
        return (true); // done with error
        break;
    }

    return (false); // not done yet
}

/*
*******************************************************************************
    Animate Fade out

    Precondition is that initial position of the Neopixels is set

    Returns true when animation is complete
*******************************************************************************
*/
bool animateFadeOut(unsigned long currentMillis) {
    static int animationStatus = 0;
    static unsigned long timeoutMillis = 0;
    static uint8_t originalBrightness = 0; // to restore after animation

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Initialize animation
        //---------------------------------
        originalBrightness = npx.brightness;
        animationStatus++;
        ESP_LOGI(TAG, "Start Fade Out animation");
        break;

    case 1:
        //---------------------------------
        //  Run the Brightness animation
        //---------------------------------
        if (fadeBrightness(currentMillis, false)) { // fade down
            animationStatus++;
            timeoutMillis = currentMillis + 1000; // delay for visibility of min brightness
        } // else: keep fading the brightness
        break;

    case 2:
        //---------------------------------
        //  Finalising
        //---------------------------------
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        npx.brightness = originalBrightness;
        animationStatus = 0;
        return (true); // indicate that animation is done
        break;

    default:
        //---------------------------------
        // Unexpected status
        //---------------------------------
        ESP_LOGE(TAG, "Unexpected FadeOut animationStatus=%d", animationStatus);
        animationStatus = 0;
        return (true); // done with error
        break;
    }
    return (false); // not done yet
}

/*
*******************************************************************************
    Brightness fade up/down animation

    Returns true when animation is complete
*******************************************************************************
*/
bool animateBrightness(unsigned long currentMillis) {
    static const int nrUpOrDownFades = 5; // nr of times brightness will be faded (either up or down)
    static int upOrDownFadeCounter = 0;

    static int animationStatus = 0;
    static unsigned long timeoutMillis = 0;
    static uint8_t originalBrightness = 0; // to restore after animation

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Initialize animation
        //---------------------------------
        ESP_LOGI(TAG, "Start Brightness animation with 4 Neopixels (R,G,B,W)");
        originalBrightness = npx.brightness;
        npx.setAllPixels(neopixelBlack);
        npx.show();
        animationStatus++;
        timeoutMillis = currentMillis + 1000; // delay for visibility of black screen
        break;

    case 1:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        // Let's go
        //---------------------------------
        animationStatus++;
        upOrDownFadeCounter = 1;
        break;

    case 2:
        //---------------------------------
        //  Run the Brightness animation
        //---------------------------------
        if (fadeBrightness(currentMillis, (upOrDownFadeCounter & 1))) { // odd: fade up, even: fade down
            if (upOrDownFadeCounter >= nrUpOrDownFades) {
                upOrDownFadeCounter = 0;
                animationStatus++;
                timeoutMillis = currentMillis + 1000; // delay for visibility of end brightness
                break;
            }
            upOrDownFadeCounter++;
            // timeoutMillis = currentMillis + 1000; // delay for visibility of this fading
        } // else: keep fading the brightness
        break;

    case 3:
        //---------------------------------
        //  Finalising
        //---------------------------------
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        npx.brightness = originalBrightness;
        animationStatus = 0;
        return (true); // indicate that animation is done
        break;

    default:
        //---------------------------------
        // Unexpected status
        //---------------------------------
        animationStatus = 0;
        return (true); // done with error
        break;
    }
    return (false); // not done yet
}
