# Level shifter

## Data signal

The ESP32xx family uses 3V3 logic, while most Neopixels expect 5V.
Therefore, better use a level shifter like the 74HCT126 between the ESP32xx data pin GPIO output and the Neopixel data input (DI).

## Enable signal

The 74HCT126 also has enable inputs that can be set high by the ESP32xx during setup, just before calling begin() of the Neopixel Driver. This could prevent startup glitches in case the used ESP32xx data output pin gets pulses during booting or programming. Obviously, you will need an extra ESP32xx output pin for this.

When NOT driving the Enable with ESP32xx, simply connect the 74HCT126 Enable pin to the +5V.
