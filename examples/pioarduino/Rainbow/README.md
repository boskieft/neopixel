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
I (238) ARDUINO: Pin 18 already has type USB_DM (39) with bus 0x3fc90908
I (238) ARDUINO: Pin 19 already has type USB_DP (40) with bus 0x3fc90908
I (5242) RAINBOW: ----- Running setup, chip=`esp32c3` -----
I (5242) RAINBOW: Using dataPin=5
I (5242) RAINBOW: Switching On enablePin=10
I (5246) RAINBOW: Init the Neopixels on pin=5 with 24 pixels
D (5252) NPIX: GRB Neopixels, seq3 timing
D (5256) I2S_: Big-Endian buffer
D (5259) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (5265) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (5274) I2S_: Sample rate=75000 frames/sec
D (5278) I2S_: Interrupt priority=0
I (5281) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (5292) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (5299) I2S_: maxSendMicros=129
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
I (8330) RAINBOW: Start the Rainbow animation
I (8330) RAINBOW: Method: recalculate each frame
I (21130) RAINBOW: Method: just rotate pixels
I (33922) RAINBOW: Method: recalculate each frame
...
```
