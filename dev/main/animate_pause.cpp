/*
*******************************************************************************
    Pause function, intended to use between Neopixel animations

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"

#include "config_neopixels.h"

static const char *TAG = "PAUSE";

/*
*******************************************************************************
    Ranges animation

    Returns true when animation is complete
*******************************************************************************
*/
bool animatePause(unsigned long currentMillis) {
    static PixelColor dimmedBlue = makePixelColor(0, 0, 0x20);
    static int animationStatus = 0;
    static unsigned long timeoutMillis = 0;

    switch (animationStatus) {
    case 0:
        //---------------------------------
        //  Show static image (all blue)
        //---------------------------------
        ESP_LOGI(TAG, "Start Pause animation: show all blue pixels");
        npx.setAllPixels(dimmedBlue);
        npx.show();
        timeoutMillis = currentMillis + 3000; // delay for visibility all blue
        animationStatus++;
        break;

    case 1:
        if ((long)(currentMillis - timeoutMillis) < 0) {
            return (false); // waiting
        }

        //---------------------------------
        //  Done
        //---------------------------------
        ESP_LOGD(TAG, "Pause done");
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
