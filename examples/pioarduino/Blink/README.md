# Neopixel Blink Example

This Example demonstrates how to blink both the Status LED (on standard GPIO) and one Neopixel at the same time.
Build and run in the `Pioarduino` environment.

## Change the GPIO numbers

The used GPIO pins are defined in file `set_gpio.h` based on selected eps32 variant you build. Most probably you have to change the GPIO pins for your specific ESP32xx hardware configuration. You can do that in `set_gpio.h`, or by NOT using that file and setting them manually in `main.cpp`.

## Install and run

See the generic instructions in the neopixel/examples/pioarduino/README.md

The Neopixel and optionally the Status LED will both blink.

The VSC Terminal will show detailed Debug logging like this:

```code
...
============ Before Setup End ============
I (3743) BLINK: ----- Running setup, chip=`esp32c3` -----
I (3743) BLINK: Using statusLedPin=8
I (3743) BLINK: Using dataPin=5
I (3746) BLINK: Switching On enablePin=10
I (3750) BLINK: Init the Neopixels on pin=5 with 1 pixels
D (3757) NPIX: GRB Neopixels, seq3 timing
D (3761) I2S_: Big-Endian buffer
D (3764) I2S_: Raw data size=9 bytes, bitRate=2400000 bps
D (3769) I2S_: Optimised buffer size=32 bytes, frames/chunk=8, bytes/frame=4, DMA chunks=2
D (3778) I2S_: Sample rate=75000 frames/sec
D (3783) I2S_: Interrupt priority=0
I (3786) I2S_: Started I2S channel=0, internal DMA buffer size=64 bytes, required transmit time=213 us
D (3796) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3803) I2S_: maxSendMicros=136
I (3806) BLINK: Start the Blink animation
=========== After Setup Start ============
...
============ After Setup End =============
```

NOTE: _(no further logging from the example code)_
