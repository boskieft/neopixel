# Neopixel Chaser Example

This Example demonstrates the Chaser effect: Neopixels are lit one after each other.
Build and run in the `ESP-IDF` environment.

## Change the GPIO numbers

To keep things simple, the used GPIO pins are defined at the top of `chaser_main.cpp`. Most probably you have to change the GPIO pins for your specific ESP32xx hardware configuration, and the number of Neopixels you have.

The `enablePin` is only needed in case your (74HCT126) level shifter has an Enable input, that needs to be driven by your ESP32xx chip. In case you do NOT have it, set `enablePin = GPIO_NUM_NC`.

## Install and run

See the generic instructions in the neopixel/examples/ESP-IDF/README.md

The Neopixels will be lit one after each other.

The VSC Terminal will show logging like this:

```text
...
I (274) main_task: Started on CPU0
I (274) main_task: Calling app_main()
I (3274) CHASER: Switching On enablePin=10
I (3274) CHASER: Starting the Neopixel driver on pin=5 with 24 pixels
D (3274) NPIX: GRB Neopixels, seq3 timing
D (3274) I2S_: Big-Endian buffer
D (3274) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3284) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3294) I2S_: Sample rate=75000 frames/sec
D (3294) I2S_: Interrupt priority=0
I (3304) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3314) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3314) I2S_: maxSendMicros=197
I (3324) CHASER: Start the Chaser animation
```
