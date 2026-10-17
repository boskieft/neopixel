# Neopixel Blink Example

This Example demonstrates how to blink just one Neopixel.
Build and run in the `ESP-IDF` environment.

## Change the GPIO numbers

To keep things simple, the used GPIO pin and PixelType are defined at the top of `blink_main.cpp`. Most probably you have to change this for your specific ESP32xx hardware configuration.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md

The Neopixel will start blinking 5x/sec.

The VSC Terminal will show logging like this:

```text
...
I (24) boot: ESP-IDF v6.1 2nd stage bootloader
I (24) boot: compile time Oct 17 2026 16:11:13
I (25) boot: chip revision: v0.4
I (25) boot: efuse block revision: v1.3
I (28) boot.esp32c3: SPI Speed      : 80MHz
I (32) boot.esp32c3: SPI Mode       : DIO
...
I (145) cpu_start: Unicore app
I (153) cpu_start: GPIO 20 and 21 are used as console UART I/O pins
I (153) cpu_start: Pro cpu start user code
I (153) cpu_start: cpu freq: 160000000 Hz
I (158) app_init: Application information:
I (163) app_init: Project name:     ESP-IDF-blink
I (168) app_init: App version:      1
I (172) app_init: Compile time:     Oct 17 2026 16:10:55
...
I (273) main_task: Started on CPU0
I (273) main_task: Calling app_main()
I (3273) BLINK: ----- Startup, chip=`esp32c3` -----
I (3273) NPIX: Beginning the Neopixel Driver on dataPin=5 with 1 Neopixels
D (3273) NPIX: GRB Neopixels, seq3 timing
D (3273) I2S_: Big-Endian buffer
D (3273) I2S_: Raw data size=9 bytes, bitRate=2400000 bps
D (3283) I2S_: Optimised buffer size=32 bytes, frames/chunk=8, bytes/frame=4, DMA chunks=2
D (3293) I2S_: Sample rate=75000 frames/sec
D (3293) I2S_: Interrupt priority=0
I (3303) I2S_: Started I2S channel=0, internal DMA buffer size=64 bytes, required transmit time=213 us
D (3313) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3313) I2S_: maxSendMicros=205
I (3323) BLINK: Start the Blink animation
...
```
