/*
*******************************************************************************
    Rainbow test function

    Show the colors of the rainbow on the Neopixels

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "config_neopixels.h"

static const char *TAG = "RAINBOW";

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
    Rainbow animation

    Returns true when animation is complete
*******************************************************************************
*/
bool animateRainbow(unsigned long currentMillis) {
    static const int nrCycles = 3; // nr of wobble cycles
    static int cycleCounter = 0;

    static int animationStatus = 0;
    static uint8_t rainbowOffset = 0; // offset for the rainbow animation
    static unsigned long timeoutMillis = 0;

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Show static image (all blue)
        //---------------------------------
        ESP_LOGI(TAG, "Start Rainbow animation");
        cycleCounter = 0;
        rainbowOffset = 0;
        animationStatus++;
        break;

    case 1:
        //---------------------------------
        //  Cycle the rainbow colors
        //---------------------------------
        rainbow(rainbowOffset);

        if (rainbowOffset == 255) {
            // Cycle done
            cycleCounter++;
            if (cycleCounter < nrCycles) {
                rainbowOffset = 0; // reset for the next cycle
            } else {
                timeoutMillis = currentMillis + 3000; // delay after completing all cycles
                animationStatus++;                    // all cycles done
            }
            break;
        }
        rainbowOffset++; // increment the offset for the next frame
        break;

    case 2:
        //---------------------------------
        //  Finalising
        //---------------------------------
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }
        animationStatus++;
        break;

    case 3:
        //---------------------------------
        //  Done
        //---------------------------------
        ESP_LOGD(TAG, "Rainbox animation done");
        animationStatus = 0;
        return (true); // indicate that animation is complete
        break;

    default:
        ESP_LOGE(TAG, "Unexpected animationStatus=%d", animationStatus);
        animationStatus = 0;
        return (true); // done with error
        break;
    }
    return (false); // not done yet
}
