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

    Using one assemby of 9 rings with GRBW NeoPixels
-----------------------------------------------------------
*/
NeopixelDriver<PixelType::GRBW_SEQ3> npx;
static const size_t ringSize = (1 + 8 + 12 + 16 + 24 + 32 + 40 + 48 + 60);

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
            walkingColor = neopixelWhite_RGBW;
            break;
        default:
            walkingColor = neopixelRed;
            colorIndex = 0;
            return (true); // indicate that a new loop will be started
            break;
        }
    }
    return (false);
}

void animateNeopixels(int64_t currentMicros) {
    static int animationStep = 0;
    static int64_t animationTimeoutMicros = 0;

    switch (animationStep) {
    case 0:
        if (walkingPixelAllColors()) {
            animationStep++;
        }
        break;

    case 1: {
        auto oldBrightness = npx.brightness;
        npx.brightness = 0x03; // set very low brightness, because all Neopixels will be lit
        npx.setAllPixels(neopixelBlue);
        npx.show();
        npx.brightness = oldBrightness;
        animationStep++;
        animationTimeoutMicros = currentMicros + 3000000; // 3 second delay before resetting the animation
        break;
    }

    default:
        if (currentMicros - animationTimeoutMicros >= 0) {
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

    int64_t currentMicros = esp_timer_get_time();
    int64_t toggleTimeoutMicros = currentMicros;

    while (1) {
        currentMicros = esp_timer_get_time();
        if (currentMicros - toggleTimeoutMicros >= 0) {
            toggleStatusLed();
            toggleTimeoutMicros = currentMicros + 100000; // 100 ms
        }
        animateNeopixels(currentMicros);
    }
}
