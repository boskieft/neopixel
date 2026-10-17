# Neopixel Range Example

This example demonstrates the usage of ranges for filling and rotating.
Build and run in the `pioarduino` environment.

## Change the GPIO numbers

The used GPIO pin to drive the Neopixel is configured at the top of the program file for my personal hardware setup. Most probably you have to change it to your own situation. The example needs at least 12 Neopixels for a nice animation.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

One half of the pixels will be dimmed white, on the other half the Chaser effect will run with 3 pixels (red, green and blue).

The Chaser pixels will not affect the white pixels.

The orientation turns 90 degrees after 3 Chaser sequences.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3806) RANGE: ----- Running setup, chip=`esp32c3` -----
I (3806) NPIX: Beginning the Neopixel Driver on dataPin=5 with 24 Neopixels
D (3808) NPIX: GRB Neopixels, seq3 timing
D (3812) I2S_: Big-Endian buffer
D (3816) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3821) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3830) I2S_: Sample rate=75000 frames/sec
D (3835) I2S_: Interrupt priority=0
I (3838) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3848) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3856) I2S_: maxSendMicros=145
I (3859) RANGE: Start the Range animation
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
```

NOTE: _(no further logging from the example code)_

## Rotate method not supported?

The original ESP32 and the ESP32-S2 have an older I2S peripheral that does NOT support `Big-Endian` encoding. This, and the odd number of bytes required for certain PixelTypes (especially `GRB_SEQ3`) causes that the Neopixel Driver cannot do in-memory rotation of the pixels. The logging in this example will show that clearly:

```code
...
I (3643) RANGE: ----- Running setup, chip=`esp32` -----
I (3644) NPIX: Beginning the Neopixel Driver on dataPin=19 with 24 Neopixels
D (3646) NPIX: GRB Neopixels, seq3 timing
D (3650) I2S_: Little-Endian buffer (ESP32, ESP32-S2)
...
I (3698) RANGE: Start the Range animation
W (3703) RANGE: This `esp32`/PixelType combination does NOT support the rotate method, so cannot do Chaser animation
```

The white half circles will be shown and still be rotated 90 degrees periodically. This is possible because the half circles are simply redrawn with their new positions, no rotation function is used for that.
