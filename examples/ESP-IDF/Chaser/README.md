# Neopixel Chaser Example

This Example demonstrates the Chaser effect: Neopixels are lit one after each other.
Build and run in the `ESP-IDF` environment.

## Change the GPIO numbers

To keep things simple, the used GPIO pins are defined at the top of `chaser_main.cpp`. Most probably you have to change the GPIO pins for your specific ESP32xx hardware configuration, and the number of Neopixels you have.

The `enablePin` is only needed in case your (74HCT126) level shifter has an Enable input, that needs to be driven by your ESP32xx chip. In case you do NOT have it, set `enablePin = GPIO_NUM_NC`.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md
