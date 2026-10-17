# Neopixel Chaser Example

This example demonstrates the chaser effect: Neopixels are lit one after another.
Build and run in the `pioarduino` environment.

## Change the GPIO numbers

The used GPIO pin to drive the Neopixel is configured at the top of the program file for my personal hardware setup. Most probably you have to change it to your own situation.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixels will be lit one after another.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (6924) CHASER: ----- Running setup, chip=`esp32c3` -----
I (6924) NPIX: Beginning the Neopixel Driver on dataPin=5 with 24 Neopixels
D (6927) NPIX: GRB Neopixels, seq3 timing
D (6931) I2S_: Big-Endian buffer
D (6934) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (6940) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (6948) I2S_: Sample rate=75000 frames/sec
D (6953) I2S_: Interrupt priority=0
I (6956) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (6966) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (6974) I2S_: maxSendMicros=144
I (6977) CHASER: Start the Chaser animation
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
```

NOTE: _(no further logging from the example code)_
