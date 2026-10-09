
#error "@@@TODO: convert from EDP-IDF to pioardiono
#include <Arduino.h>
#include "set_gpio.h" // set in comment to define the data pin manually below

#ifndef SET_GPIO_H
// instead of using set_gpio() you can also set it manually here
gpio_num_t dataPin = GPIO_NUM_21;

void set_gpio(void) { return (true); } // dummy
#endif

/*
-------------------------------------------------------------------------------
    Start the Neopixel driver
-------------------------------------------------------------------------------
*/
#define PIXEL_COUNT 24 // nr of Neopixels to drive
NeopixelDriver<PixelType::GRB_SEQ3> npx;

bool startNeopixelDriver(void) {
    if (!set_gpio()) {
        return (false);
    }

    ESP_LOGI(TAG, "Init the Neopixels on pin=%d with %d pixels", dataPin, PIXEL_COUNT);
    npx.begin(PIXEL_COUNT, dataPin);
    //@@@TODO: error handling

    npx.setAllPixels(neopixelBlack); // set all pixels to black
    npx.show();                      // send the data to the Neopixels
    npx.brightness = 0x10;           // medium brightness
    return (true);
}

/*
-------------------------------------------------------------------------------
    Chaser animation
-------------------------------------------------------------------------------
*/
void chaserAnimation(void) {
    static int coloredIndex = 0;
    static int blackIndex = PIXEL_COUNT - 1;

    npx.setPixel(blackIndex, neopixelBlack); // erase previously colored pixel
    npx.setPixel(coloredIndex, neopixelRed); // set new colored pixel
    npx.show();

    // Update the pixel indexes for the next iteration
    blackIndex = coloredIndex;
    if (++coloredIndex >= PIXEL_COUNT) {
        coloredIndex = 0; // new loop
    }
}

/*
***********************************************************
    Main
***********************************************************
 */
void app_main(void) {
    vTaskDelay(3000 / portTICK_PERIOD_MS); // allow Terminal to connect
    startNeopixelDriver();

    ESP_LOGI(TAG, "Start the Chaser animation");

    while (1) {
        chaserAnimation();
        vTaskDelay(10 / portTICK_PERIOD_MS); // delay for visibility
    }
}
