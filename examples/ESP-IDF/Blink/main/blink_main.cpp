/*
***********************************************************
    Blink example

    Neopixel and Status LED both blinking
***********************************************************
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "neopixel.h"

static const char *TAG = "BLINK";

/*
-----------------------------------------------------------
    GPIO pins -> Adapt to your hardware configuration !
-----------------------------------------------------------
*/
// My ESP32-C3 config
static const gpio_num_t statusLedPin = GPIO_NUM_8; // output pin to drive the classic on/off Status LED

static const gpio_num_t enablePin = GPIO_NUM_10; // optional output pin to enable the 74HCT126 level shifter
static const gpio_num_t dataPin = GPIO_NUM_5;    // output data pin to DI of the Neopixels (via 74HCT126 level shifter)

/*
-----------------------------------------------------------
    Classic on/off Status LED
-----------------------------------------------------------
*/
static void configureStatusLed(void) {
    ESP_LOGI(TAG, "Status LED on GPIO=%d", statusLedPin);
    gpio_reset_pin(statusLedPin);
    gpio_set_direction(statusLedPin, GPIO_MODE_OUTPUT);
}

static void blinkStatusLed(bool isOn) {
    gpio_set_level(statusLedPin, !isOn); // LED is active low
}

/*
-----------------------------------------------------------
    Neopixel LED
-----------------------------------------------------------
*/
NeopixelDriver<PixelType::GRB_SEQ3> npx;
#define PIXEL_COUNT 1 // just one (1) pixel for this example

void startNeopixel(void) {
    // Optional: Enable the level shifter
    if (enablePin != GPIO_NUM_NC) {
        ESP_LOGI(TAG, "Switching On enablePin=%d", enablePin);
        gpio_set_direction(enablePin, GPIO_MODE_OUTPUT);
        gpio_set_level(enablePin, 1);
    } else {
        ESP_LOGI(TAG, "Optional enablePin is NOT configured");
    }

    // Optional: Increase logging of the Neopixel driver and its I2S subsystem
    esp_log_level_set("NPIX", ESP_LOG_DEBUG);
    esp_log_level_set("I2S_", ESP_LOG_DEBUG);

    // Start the Neopixel driver
    ESP_LOGI(TAG, "Starting the Neopixel driver on pin=%d with %d pixels", dataPin, PIXEL_COUNT);
    npx.begin(PIXEL_COUNT, dataPin);
    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
}

void blinkNeopixel(bool isOn) {
    if (isOn) {
        npx.setPixel(0, neopixelRed);
    } else {
        npx.setPixel(0, neopixelBlack);
    }
    npx.show();
}

/*
***********************************************************
    Main
***********************************************************
 */
extern "C" void app_main(void) {
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect
    configureStatusLed();
    startNeopixel();

    bool isOn = true;

    ESP_LOGI(TAG, "Start the Blink animation");

    while (1) {
        blinkStatusLed(isOn);
        blinkNeopixel(isOn);

        /* Toggle the LED state */
        isOn = !isOn;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
