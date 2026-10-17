# Neopixel Chaser Example

This Example demonstrates the chaser effect: Neopixels are lit one after another in a chasing sequence.
Build and run in the `ESP-IDF` environment.

## Change the GPIO numbers

To keep things simple, the used GPIO pin and PixelType are defined at the top of `chaser_main.cpp`. Most probably you have to change this for your specific ESP32xx hardware configuration.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md

The Neopixels will be lit one after another.

The VSC Terminal will show logging like this:

```text
...
I (273) main_task: Started on CPU0
I (273) main_task: Calling app_main()
I (3273) CHASER: ----- Startup, chip=`esp32c3` -----
I (3273) NPIX: Beginning the Neopixel Driver on dataPin=5 with 24 Neopixels
D (3273) NPIX: GRB Neopixels, seq3 timing
D (3273) I2S_: Big-Endian buffer
D (3273) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3283) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3293) I2S_: Sample rate=75000 frames/sec
D (3293) I2S_: Interrupt priority=0
I (3303) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3313) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3313) I2S_: maxSendMicros=207
I (3323) CHASER: Start the Chaser animation
...
```
