# Neopixel Chaser Example

This Example demonstrates the chaser effect: Neopixels are lit one after each other.
Build and run in the `Pioarduino` environment.

## Change the GPIO numbers

The used GPIO pins are defined in file `set_gpio.h` based on selected eps32 variant you build. Most probably you have to change the GPIO pins for your specific ESP32xx hardware configuration. You can do that in `set_gpio.h`, or by NOT using that file and setting them manually in `main.cpp`.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixels will be lit one by one.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3259) CHASER: ----- Running setup, chip=`esp32c3` -----
I (3259) CHASER: Using dataPin=5
I (3259) CHASER: Switching On enablePin=10
I (3263) CHASER: Init the Neopixels on pin=5 with 24 pixels
D (3269) NPIX: GRB Neopixels, seq3 timing
D (3273) I2S_: Big-Endian buffer
D (3276) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3282) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3290) I2S_: Sample rate=75000 frames/sec
D (3295) I2S_: Interrupt priority=0
I (3298) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3308) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3316) I2S_: maxSendMicros=133
I (3319) CHASER: Start the Chaser animation
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
```

NOTE: _(no further logging from the example code)_
