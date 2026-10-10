# Neopixel Blink Example

This Example demonstrates how to blink both the Status LED (on standard GPIO) and one Neopixel at the same time.
Build and run in the `ESP-IDF` environment.

## Change the GPIO numbers

To keep things simple, the used GPIO pins are defined at the top of `blink_main.cpp`. Most probably you have to change these GPIO pins for your specific ESP32xx hardware configuration.

The `enablePin` is only needed in case your (74HCT126) level shifter has an Enable input, that needs to be driven by your ESP32xx chip. In case you do NOT have it, set `enablePin = GPIO_NUM_NC`.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md

Both the (classic, GPIO) Status LED and the Neopixel will start blinking.

The VSC Terminal will show logging like this:

```text
...
I (24) boot: ESP-IDF v6.1 2nd stage bootloader
I (24) boot: compile time Oct  8 2026 23:20:30
I (25) boot: chip revision: v0.4
I (25) boot: efuse block revision: v1.3
I (28) boot.esp32c3: SPI Speed      : 80MHz
I (32) boot.esp32c3: SPI Mode       : DIO
...
I (145) cpu_start: Unicore app
I (153) cpu_start: GPIO 20 and 21 are used as console UART I/O pins
I (154) cpu_start: Pro cpu start user code
I (154) cpu_start: cpu freq: 160000000 Hz
I (158) app_init: Application information:
I (163) app_init: Project name:     ESP-IDF-blink
I (168) app_init: App version:      1
I (173) app_init: Compile time:     Oct  8 2026 23:20:17
...
I (274) main_task: Started on CPU0
I (274) main_task: Calling app_main()
I (3274) BLINK: Status LED on GPIO=8
I (3274) BLINK: Switching On enablePin=10
I (3274) BLINK: Starting the Neopixel driver on pin=5 with 1 pixels
D (3274) NPIX: GRB Neopixels, seq3 timing
D (3274) I2S_: Big-Endian buffer
D (3284) I2S_: Raw data size=9 bytes, bitRate=2400000 bps
D (3284) I2S_: Optimised buffer size=32 bytes, frames/chunk=8, bytes/frame=4, DMA chunks=2
D (3294) I2S_: Sample rate=75000 frames/sec
D (3304) I2S_: Interrupt priority=0
I (3304) I2S_: Started I2S channel=0, internal DMA buffer size=64 bytes, required transmit time=213 us
D (3314) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3324) I2S_: maxSendMicros=195
I (3324) BLINK: Start the Blink animation
```
