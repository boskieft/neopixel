/*
***********************************************************
    Blink example
***********************************************************
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "neopixel.h"
#include "esp_timer.h"

static const char *TAG = "BLINK";

/*
-----------------------------------------------------------
    GPIO pins

    Set automatically to the correct pins on my hardware,
    based on the selected ESP32xx variant
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
#error " ESP32xx target device has not been set, or is unknown"
#endif

/*
-----------------------------------------------------------
    Classic on/off Status LED as heartbeat
-----------------------------------------------------------
*/
static void configureStatusLed(void) {
    ESP_LOGI(TAG, "Status LED on GPIO=%d", statusLedPin);
    gpio_reset_pin(statusLedPin);
    gpio_set_direction(statusLedPin, GPIO_MODE_OUTPUT);
}

static void toggleStatusLed(void) {
    static bool isOn = false;
    gpio_set_level(statusLedPin, isOn);
    isOn = !isOn;
}

/*
-----------------------------------------------------------
    Neopixels
-----------------------------------------------------------
*/
#if (1 == 1)
// 1 assemby of 9 rings with GRBW NeoPixels
NeopixelDriver<PixelType::GRBW_SEQ3> npx;
static const size_t ringSize = (1 + 8 + 12 + 16 + 24 + 32 + 40 + 48 + 60);
#else
// 1 ring of 60, 1 ring of 24, 1 assembly of 6 rings
NeopixelDriver<PixelType::GRB_SEQ3> npx;
static const size_t ringSize = (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32);
#endif

static const size_t rotateSteps = ringSize / 4; // number of steps to showcase the rotation

void startNeopixel(void) {
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

    npx.begin(ringSize, dataPin);    // just one (1) pixel for this example
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixel ring
    npx.brightness = 0x10;           // medium brightness
}

bool walkingPixel(PixelColor walkingColor) {
    static int coloredIndex = 0;
    static int blackIndex = ringSize - 1;

    npx.setPixel(blackIndex, neopixelBlack);  // erase previously colored pixel
    npx.setPixel(coloredIndex, walkingColor); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= ringSize) {
        // New loop
        coloredIndex = 0;
        return (true); // indicate that a new loop will be started
    }
    return (false);
}

bool walkingPixelAllColors(void) {
    static int colorIndex = 0;
    static PixelColor walkingColor = neopixelRed;

    if (walkingPixel(walkingColor)) {
        switch (colorIndex++) {
        case 0:
            walkingColor = neopixelGreen;
            break;
        case 1:
            walkingColor = neopixelBlue;
            break;
        case 2:
            if (npx.hasWhite()) {
                walkingColor = neopixelWhite_RGBW;
                break;
            }
            [[fallthrough]]; // skip White, proceeed with next
        default:
            walkingColor = neopixelRed;
            colorIndex = 0;
            return (true); // indicate that a new loop will be started
            break;
        }
    }
    return (false);
}

bool rotateLeft(unsigned long currentMillis) {
    static int steps = rotateSteps;
    static unsigned long animationTimeoutMillis = 0;
    if ((long)(currentMillis - animationTimeoutMillis) >= 0) {
        steps--;
        if (steps < 0) {
            steps = rotateSteps; // reset the steps for the next rotation
            return true;         // indicate that the rotation is complete
        } else {
            npx.rotateLeft();
            npx.show();
            animationTimeoutMillis = currentMillis + 100; // slow rotation
        }
    }
    return false; // Placeholder return value
}

bool rotateRight(unsigned long currentMillis) {
    static int steps = rotateSteps;
    static unsigned long animationTimeoutMillis = 0;
    if ((long)(currentMillis - animationTimeoutMillis) >= 0) {
        steps--;
        if (steps < 0) {
            steps = rotateSteps; // reset the steps for the next rotation
            return true;         // indicate that the rotation is complete
        } else {
            npx.rotateRight();
            npx.show();
            animationTimeoutMillis = currentMillis + 100; // slow rotation
        }
    }
    return false; // Placeholder return value
}

void animateNeopixels(unsigned long currentMillis) {
    static int animationStep = 0;
    static unsigned long animationTimeoutMillis = 0;

    switch (animationStep) {
    case 0:
        if (walkingPixelAllColors()) {
            if (npx.isRotatable())
                animationStep++;
            else
                animationStep += 3; // skip left and right rotations
        }
        break;

    case 1:
        if (rotateLeft(currentMillis)) {
            animationStep++;
        }
        break;

    case 2:
        if (rotateRight(currentMillis)) {
            animationStep++;
        }
        break;

    case 3: {
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
***********************************************************
    Main
***********************************************************
 */
extern "C" void app_main(void) {
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
        animateNeopixels(currentMillis);
        vTaskDelay(1); // to prevent watchdog reset
    }
}
