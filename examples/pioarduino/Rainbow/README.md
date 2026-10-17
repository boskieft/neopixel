# Neopixel Rainbow Example

This example demonstrates the rainbow effect: the colors of the rainbow will be shifted/rotated across the Neopixels.
Two methods are demonstrated: recalculating each frame and just rotating the pixels.

Build and run in the `pioarduino` environment.

## Change the GPIO numbers

The used GPIO pin to drive the Neopixel is configured at the top of the program file for my personal hardware setup. Most probably you have to change it to your own situation.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixels will show a shifting/rotating rainbow. At each round the method is changed (either recalculate or just rotate), in the logging you can see which one is active. At a low number of Neopixels (say upto 24), the rotation method is less smooth than recalulating each frame.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3259) RAINBOW: ----- Running setup, chip=`esp32c3` -----
I (3259) NPIX: Beginning the Neopixel Driver on dataPin=5 with 24 Neopixels
D (3262) NPIX: GRB Neopixels, seq3 timing
D (3266) I2S_: Big-Endian buffer
D (3269) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3275) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3283) I2S_: Sample rate=75000 frames/sec
D (3288) I2S_: Interrupt priority=0
I (3291) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3301) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3309) I2S_: maxSendMicros=147
I (3312) RAINBOW: Start the Rainbow animation
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
I (3345) RAINBOW: Method: recalculate each frame
I (16145) RAINBOW: Method: just rotate pixels
I (28942) RAINBOW: Method: recalculate each frame
I (41742) RAINBOW: Method: just rotate pixels
...
```

## Rotate method not supported?

The original ESP32 and the ESP32-S2 have an older I2S peripheral that does NOT support `Big-Endian` encoding. This, and the odd number of bytes required for certain PixelTypes (especially `GRB_SEQ3`) causes that the Neopixel Driver cannot do in-memory rotation of the pixels. The logging in this example will show that clearly:

```code
============ Before Setup End ============
I (3643) RAINBOW: ----- Running setup, chip=`esp32` -----
...
D (3650) I2S_: Little-Endian buffer (ESP32, ESP32-S2)
...
============ After Setup End =============
I (3812) RAINBOW: Method: recalculate each frame
W (16613) RAINBOW: SKIP: This `esp32`/PixelType combination does NOT support the rotate method
I (16613) RAINBOW: Method: recalculate each frame
W (29417) RAINBOW: SKIP: This `esp32`/PixelType combination does NOT support the rotate method
I (29417) RAINBOW: Method: recalculate each frame
...
```
