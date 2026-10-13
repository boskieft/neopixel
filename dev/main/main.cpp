/*
*******************************************************************************
    Test program, to showcase all major functions of the Neopixel driver

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
*******************************************************************************
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "neopixel.h"
#include "esp_timer.h"

static const char *TAG = "MAIN";

/*
-----------------------------------------------------------
    GPIO pins

    Set automatically to the correct pins on my hardware,
    based on the selected "Espressif Device Target"
-----------------------------------------------------------
*/
#if (CONFIG_IDF_TARGET_ESP32)
static const gpio_num_t statusLedPin = GPIO_NUM_16; // output pin to drive the classic on/off status LED

static const gpio_num_t enablePin = GPIO_NUM_26; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_19;   // output data pin to DI of Neopixel ring (via 74HCT126 level shifter)
#elif (CONFIG_IDF_TARGET_ESP32S2)
static const gpio_num_t statusLedPin = GPIO_NUM_16; // output pin to drive the classic on/off status LED

static const gpio_num_t enablePin = GPIO_NUM_5; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_9;   // output data pin to DI of Neopixel ring (via 74HCT126 level shifter)
#elif (CONFIG_IDF_TARGET_ESP32S3)
static const gpio_num_t statusLedPin = GPIO_NUM_12; // output pin to drive the classic on/off status LED

static const gpio_num_t enablePin = GPIO_NUM_16; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_17;   // output data pin to DI of Neopixel ring (via 74HCT126 level shifter)
#elif (CONFIG_IDF_TARGET_ESP32C3)
static const gpio_num_t statusLedPin = GPIO_NUM_8; // output pin to drive the classic on/off status LED

static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of Neopixel ring (via 74HCT126 level shifter)
#elif (CONFIG_IDF_TARGET_ESP32C6)
static const gpio_num_t statusLedPin = GPIO_NUM_15; // output pin to drive the classic on/off status LED

static const gpio_num_t enablePin = GPIO_NUM_21; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_2;    // output data pin to DI of Neopixel ring (via 74HCT126 level shifter)
#else
#error "ESP32xx target device has not been set, or is unknown here"
#endif

/*
===============================================================================
    Start the Neopixel driver
===============================================================================
*/
#if (1 == 1)
// 1 assemby of 9 rings with GRBW NeoPixels
NeopixelDriver<PixelType::GRBW_SEQ3> npx;
static const size_t nrNeopixels = (1 + 8 + 12 + 16 + 24 + 32 + 40 + 48 + 60);
#elif (2 == 0)
// 1 ring of 60, 1 ring of 24, 1 assembly of 6 rings
NeopixelDriver<PixelType::GRB_SEQ3> npx;
static const size_t nrNeopixels = (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32);
#else
// 1 ring of 24
NeopixelDriver<PixelType::GRB_SEQ3> npx;
static const size_t nrNeopixels = 24;
#endif

static const size_t rotateSteps = nrNeopixels / 4; // number of steps to showcase the rotation

static void startNeopixel(void) {
    // Optional: set Enable output to High
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }

    // Increase logging of the Neopixel driver and I2S subsystem
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);

    // Init the driver
    ESP_LOGI(TAG, "Init the Neopixel Driver on dataPin=%d with nrNeopixels=%d", dataPin, nrNeopixels);
    npx.begin(nrNeopixels, dataPin); // just one (1) pixel for this example
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixel ring
    npx.brightness = 0x10;           // medium brightness
}

/*
===============================================================================
    Check and log the Neopixel statistics
===============================================================================
*/
static void checkStatistics(void) {

    //---------------------------------------------------------------------------------------------
    // Maximum time taken to send data to the Neopixels
    //
    // Initially short (approx 200 us)
    // Will increase when you try to send faster than the Neopixel transmission rate,
    // because the Driver needs to wait for the previous transmission to complete
    //---------------------------------------------------------------------------------------------
    if (esp_log_level_get("I2S_") < ESP_LOG_DEBUG) {
        // Driver is not logging it, do it here
        static int loggedMaxSendMicros = 0;
        if (npx.stats.maxSendMicros > loggedMaxSendMicros) {
            loggedMaxSendMicros = npx.stats.maxSendMicros;
            ESP_LOGI(TAG, "maxSendMicros=%d", loggedMaxSendMicros);
        }
    } // else: don't log twice

    //---------------------------------------------------------------------------------------------
    // Used chunks
    //
    // Usually same as the configured number of chunks (2 or more)
    // Will increase on certain ESP32xx variants when sending fast to a small number of Neopixels,
    // because the Driver may need to send multiple dummy flush chunks until it is done
    //---------------------------------------------------------------------------------------------
    static int loggedMaxChunksSent = 0;
    if (npx.stats.maxNrChunksSent > loggedMaxChunksSent) {
        loggedMaxChunksSent = npx.stats.maxNrChunksSent;
        ESP_LOGI(TAG, "maxNrChunksSent=%d", loggedMaxChunksSent);
    }

    //---------------------------------------------------------------------------------------------
    // Preload data errors
    //
    // Never seen this happen in practice
    //---------------------------------------------------------------------------------------------
    static int loggedNrPreloadDataErrors = 0;
    if (npx.stats.nrPreloadDataErrors > loggedNrPreloadDataErrors) {
        loggedNrPreloadDataErrors = npx.stats.nrPreloadDataErrors;
        ESP_LOGI(TAG, "nrPreloadDataErrors=%d", loggedNrPreloadDataErrors);
    }
    //---------------------------------------------------------------------------------------------
    // Channel errors
    //
    // Never seen this happen in practice
    //---------------------------------------------------------------------------------------------
    static int loggedNrChannelErrors = 0;
    if (npx.stats.nrChannelErrors > loggedNrChannelErrors) {
        loggedNrChannelErrors = npx.stats.nrChannelErrors;
        ESP_LOGI(TAG, "nrChannelErrors=%d", loggedNrChannelErrors);
    }

    //---------------------------------------------------------------------------------------------
    // Timeouts
    //
    // Only seen this one happen in case the dataPin was not configured correctly
    //---------------------------------------------------------------------------------------------
    static int loggedNrTimeouts = 0;
    if (npx.stats.nrTimeouts > loggedNrTimeouts) {
        loggedNrTimeouts = npx.stats.nrTimeouts;
        ESP_LOGI(TAG, "nrTimeouts=%d", loggedNrTimeouts);
    }

#if (ENABLE_I2S_TASK_VERSION)
    //---------------------------------------------------------------------------------------------
    // Free room in the Task stack
    //
    // Should remain at least 100 bytes, ESP32xx will reboot when the stack is exhausted
    // Depends a lot on the ESP32xx variant, Task on ESP32-S3 uses the most stack
    //---------------------------------------------------------------------------------------------
    static UBaseType_t loggedMinFreeTaskStack = UINT_MAX;
    if (npx.stats.minimumFreeStack < loggedMinFreeTaskStack) {
        loggedMinFreeTaskStack = npx.stats.minimumFreeStack;
        ESP_LOGI(TAG, "minimumFreeStack=%u", loggedMinFreeTaskStack);
    }
#endif
}

/*
===============================================================================
    Neopixels animations
===============================================================================
*/
static bool chaserAnimation(PixelColor chaserColor) {
    static int coloredIndex = 0;
    static int blackIndex = nrNeopixels - 1;

    npx.setPixel(blackIndex, neopixelBlack); // erase previously colored pixel
    npx.setPixel(coloredIndex, chaserColor); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= nrNeopixels) {
        // New loop
        coloredIndex = 0;
        return (true); // indicate that a new loop will be started
    }
    return (false);
}

static bool chaserAnimationAllColors(void) {
    static int colorIndex = 0;
    static PixelColor chaserColor = neopixelRed;

    if (chaserAnimation(chaserColor)) {
        switch (colorIndex++) {
        case 0:
            chaserColor = neopixelGreen;
            break;
        case 1:
            chaserColor = neopixelBlue;
            break;
        case 2:
            if (npx.hasWhite()) {
                chaserColor = neopixelWhite_RGBW;
                break;
            }
            [[fallthrough]]; // skip White, proceeed with next
        default:
            chaserColor = neopixelRed;
            colorIndex = 0;
            return (true); // indicate that Chaser animation is done
            break;
        }
    }
    return (false); // not ready yet
}

static bool rotateLeftAnimation(unsigned long currentMillis) {
    static int steps = rotateSteps;
    static unsigned long animationTimeoutMillis = 0;
    if ((long)(currentMillis - animationTimeoutMillis) >= 0) {
        steps--;
        if (steps < 0) {
            steps = rotateSteps; // reset the steps for the next rotation
            return true;         // indicate that Rotate Left animation is done
        } else {
            npx.rotateAllLeft();
            npx.show();
            animationTimeoutMillis = currentMillis + 100; // slow rotation
        }
    }
    return false; // not ready yet
}

static bool rotateRightAnimation(unsigned long currentMillis) {
    static int steps = rotateSteps;
    static unsigned long animationTimeoutMillis = 0;
    if ((long)(currentMillis - animationTimeoutMillis) >= 0) {
        steps--;
        if (steps < 0) {
            steps = rotateSteps; // reset the steps for the next rotation
            return true;         // indicate that  Rotate Right animation is done
        } else {
            npx.rotateAllRight();
            npx.show();
            animationTimeoutMillis = currentMillis + 100; // slow rotation
        }
    }
    return false; // not ready yet
}

static void animateNeopixels(unsigned long currentMillis) {
    static int animationStep = 0;
    static unsigned long animationTimeoutMillis = 0;

    switch (animationStep) {
    case 0:
        if (chaserAnimationAllColors()) {

            if (npx.isRotatable())
                animationStep++;
            else
                animationStep += 3; // skip left and right rotations
        }
        break;

    case 1:
        if (rotateLeftAnimation(currentMillis)) {
            animationStep++;
        }
        break;

    case 2:
        if (rotateRightAnimation(currentMillis)) {
            animationStep++;
        }
        break;

    case 3: {
        // All blue
        auto oldBrightness = npx.brightness;
        npx.brightness = 0x03; // set very low brightness, because all Neopixels will be lit
        npx.setAllPixels(neopixelBlue);
        npx.show();
        npx.brightness = oldBrightness;
        animationStep++;
        animationTimeoutMillis = currentMillis + 3000; // 3 second delay to show this
        break;
    }

    default:
        if ((long)(currentMillis - animationTimeoutMillis) >= 0) {
            animationStep = 0; // reset the animation step to loop the animation
        }
        break;
    }
}

/*
===============================================================================
    Classic on/off Status LED as heartbeat
===============================================================================
*/
static void configureStatusLed(void) {
    if (statusLedPin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Status LED on GPIO=%d", statusLedPin);
        gpio_reset_pin(statusLedPin);
        gpio_set_direction(statusLedPin, GPIO_MODE_OUTPUT);
    } else {
        ESP_LOGI(TAG, "Optional Status LED is NOT configured");
    }
}

static void toggleStatusLed(void) {
    if (statusLedPin != GPIO_NUM_NC) {
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
    ESP_LOGI(TAG, "----- Starting application, chip=`%s` -----", CONFIG_IDF_TARGET);

    configureStatusLed();
    startNeopixel();

    unsigned long currentMillis = esp_timer_get_time() / 1000;
    unsigned long toggleTimeoutMillis = currentMillis;

    while (1) {
        currentMillis = esp_timer_get_time() / 1000;
        if ((long)(currentMillis - toggleTimeoutMillis) >= 0) {
            toggleStatusLed();
            toggleTimeoutMillis = currentMillis + 100;
        }
        checkStatistics();
        animateNeopixels(currentMillis);
        vTaskDelay(1); // to prevent watchdog reset
    }
}
