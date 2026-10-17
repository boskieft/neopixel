# Neopixel Blink Example

This example demonstrates how to blink one (1) Neopixel.
Build and run in the `pioarduino` environment.

## Change the GPIO numbers

The used GPIO pin to drive the Neopixel is configured at the top of the program file for my personal hardware setup. Most probably you have to change it to your own situation.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixel will blink 5x/sec.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3259) BLINK: ----- Running setup, chip=`esp32c3` -----
I (3259) NPIX: Beginning the Neopixel Driver on dataPin=5 with 1 Neopixels
D (3261) NPIX: GRB Neopixels, seq3 timing
D (3265) I2S_: Big-Endian buffer
D (3269) I2S_: Raw data size=9 bytes, bitRate=2400000 bps
D (3274) I2S_: Optimised buffer size=32 bytes, frames/chunk=8, bytes/frame=4, DMA chunks=2
D (3283) I2S_: Sample rate=75000 frames/sec
D (3287) I2S_: Interrupt priority=0
I (3290) I2S_: Started I2S channel=0, internal DMA buffer size=64 bytes, required transmit time=213 us
D (3300) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3308) I2S_: maxSendMicros=147
I (3311) BLINK: Start the Blink animation
=========== After Setup Start ============
...
============ After Setup End =============
```

NOTE: _(no further logging from the example code)_
