# Neopixel Blink Example

This Example demonstrates how to blink both the Status LED (on standard GPIO) and one Neopixel at the same time.

## Change the GPIO numbers

To keep things simple, the used GPIO pins are defined at the top of `blink_example_main.cpp`. Most probably you have to change the numbers for your specific ESP32xx hardware configuration.

The `enablePin` is only needed in case your (74HCT126) level shifter has an Enable input, that needs to be driven by your ESP32xx chip. In case you do NOT have it, set `enablePin = GPIO_NUM_NC`.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md

Both the (classic, GPIO) status LED and the Neopixel will start blinking.

The VSC Terminal will show logging like this:

```text
...
I (5) boot: ESP-IDF v6.1 2nd stage bootloader
I (5) boot: compile time Oct  6 2026 21:18:14
I (6) boot: chip revision: v0.1
...
I (133) app_init: Application information:
I (137) app_init: Project name:     ESP-IDF-blink
I (142) app_init: App version:      1
I (145) app_init: Compile time:     Oct  6 2026 21:17:59
...
I (224) main_task: Started on CPU0
I (224) main_task: Calling app_main()
I (224) BLINK: Status LED on GPIO=15
I (224) BLINK: Switching On enablePin=21
D (234) NPIX: GRB Neopixels, seq3 timing
D (234) I2S_: Big-Endian buffer
D (234) I2S_: Raw data size=9 bytes, bitRate=2400000 bps
D (244) I2S_: Optimised buffer size=32 bytes, frames/chunk=8, bytes/frame=4, DMA chunks=2
D (254) I2S_: Sample rate=75000 frames/sec
D (254) I2S_: Interrupt priority=0
I (254) I2S_: Started I2S channel=0, internal DMA buffer size=64 bytes, required transmit time=213 us
D (264) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (274) I2S_: maxSendMicros=185
```
