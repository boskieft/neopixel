# Development

This "neopixel" repository is an ESP-IDF component (aka: library), NOT a standalone ESP-IDF application.

However, the `dev` directory (here) also contains the ESP-IDF Test Program, that can be used to compile and run the component on target.

## Copy/Clone

Copy or clone the whole Neopixel Driver repository from GitHub into your local `neopixel` project folder.

## Make link

**\/!\\ ATTENTION:** First time after download or clone, you must make a link in the `dev\components` folder.

See instructions in README.md of that folder.

NOTE: To publish a release to the ESP Registry, this link must be removed (to prevent an infinite loop while collecting the files).

## Configure, Build, Run

Open the `neopixel` folder in VSC.

Next, see the generic instructions in `examples\ESP-IDF\README.md` on how the build with ESP-IDF.

Do note that:

* The file structure is somewhat different with the Test Program in the `dev` folder
* The `examples` folder is not used for the Test Program

With an ESP32-C3 chip, the VSC [Terminal] will show logging like this:

```code
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM10 instead...
...
rst:0x15 (USB_UART_CHIP_RESET),boot:0xf (SPI_FAST_FLASH_BOOT)
...
I (278) main_task: Started on CPU0
I (278) main_task: Calling app_main()
I (3278) MAIN: ----- Starting Test Program, chip=`esp32c3` -----
I (3278) MAIN: Status LED configured on pin=8
I (3278) MAIN: Switching On enablePin=10
I (3278) NPIX: Beginning the Neopixel Driver on dataPin=5 with 24 Neopixels
D (3288) NPIX: GRB Neopixels, seq3 timing
D (3288) I2S_: Big-Endian buffer
D (3298) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (3298) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (3308) I2S_: Sample rate=75000 frames/sec
D (3308) I2S_: Interrupt priority=0
I (3318) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (3328) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (3338) I2S_: maxSendMicros=182
I (3338) STAT: maxNrChunksSent=2
I (3338) STAT: minimumFreeStack=828
...
```

NOTE: In app_main() the log level of individual parts can be set to Debug to see more logging.
