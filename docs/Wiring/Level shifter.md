# Level shifter

## Data signal

The ESP32xx familiy uses 3V3 logic, while most Neopixels expect 5V.
Therfore better use a level shift like the 74HCT126 between the ESP32xx datapin GPIO output and the Neopixel data input (DI).

## Enable signal

The 74HCT126 also has Enable inputs, that can be set High by the ESP32xx during setup, just before calling begin() of the Neopixel driver. This could prevent startup gliches, in case the used ESP32xx data output pin gets pulses during booting or programming. Obviously you will need an extra ESP32xx output pin for this.

When NOT driving the Enable with ESP32xx, simply connect the 74HCT126 Enable pin to the +5V.
