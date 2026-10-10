# Neopixel Rainbow Example

This Example demonstrates the Rainbow effect: the colors of the rainbox will be rotated across the Neopixels.
Two methods are demonstrated: recalculating each frame and just rotating the pixels.

Build and run in the `Pioarduino` environment.

## Change the GPIO numbers

The used GPIO pins are defined in file `set_gpio.h` based on selected eps32 variant you build. Most probably you have to change the GPIO pins for your specific ESP32xx hardware configuration. You can do that in `set_gpio.h`, or by NOT using that file and setting them manually in `main.cpp`.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixels will show a rotating rainbow. At each round the method is changed (either recalculate or just rotate), in the logging you can see which one is active.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3259) RAINBOW: ----- Running setup, chip=`esp32c3` -----
I (3259) RAINBOW: Using dataPin=5
I (3259) RAINBOW: Switching On enablePin=10
I (3263) RAINBOW: Init the Neopixels on pin=5 with 24 pixels
D (3269) NPIX: GRB Neopixels, seq3 timing
D (3273) I2S_: Big-Endian buffer
D (3276) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3282) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3291) I2S_: Sample rate=75000 frames/sec
D (3295) I2S_: Interrupt priority=0
I (3298) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3309) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3316) I2S_: maxSendMicros=131
I (3319) RAINBOW: Start the Rainbow animation
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
I (3353) RAINBOW: Method: recalculate each frame
I (16153) RAINBOW: Method: just rotate pixels
I (28950) RAINBOW: Method: recalculate each frame
I (41750) RAINBOW: Method: just rotate pixels
...
```

## Rotate method not supported?

The original ESP32 and the ESP32-S2 have an older I2S peripheral that does NOT support `Big-Endian` encoding. This, and the odd number of bytes required for certain PixelTypes (especially `GRB_SEQ3`) causes that the Neopixel driver cannot do in-memory rotation of the pixels. The logging in this example will show that clearly:

```code
============ Before Setup End ============
I (3644) BLINK: ----- Running setup, chip=`esp32` -----
...
D (3663) I2S_: Little-Endian buffer (ESP32, ESP32-S2)
...
============ After Setup End =============
I (3829) RAINBOW: Method: recalculate each frame
W (16630) RAINBOW: SKIP: This `esp32`/PixelType combination does NOT support the rotate method
I (16630) RAINBOW: Method: recalculate each frame
W (29434) RAINBOW: SKIP: This `esp32`/PixelType combination does NOT support the rotate method
I (29434) RAINBOW: Method: recalculate each frame
W (42238) RAINBOW: SKIP: This `esp32`/PixelType combination does NOT support the rotate method
I (42238) RAINBOW: Method: recalculate each frame
...
```
NOTE: _(no further logging from the example code)_
