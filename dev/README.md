# Development

This "neopixel" repository is an ESP-IDF component (aka: library), NOT a standalone ESP-IDF application.

However, the `dev` directory (here) also contains a small ESP-IDF Test Program, that can be used to compile and run the component on target.

## Copy/Clone

Copy or clone the whole Neopixel Driver repository from GitHub into your local `neopixel` project folder.

## Make link

**\/!\\ ATTENTION:** First time after download or clone, you must make a link in the `dev\components` folder.

See instructions in README.md of that folder.

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
I (276) main_task: Started on CPU0
I (276) main_task: Calling app_main()
I (276) MAIN: ----- Starting application, chip=`esp32c3` -----
I (286) MAIN: Status LED on GPIO=8
I (286) MAIN: Switching On enablePin=10
I (296) MAIN: Init the Neopixels on pin=5 with 241 pixels
D (296) NPIX: GRBW Neopixels, seq3 timing
D (306) I2S_: Big-Endian buffer
D (306) I2S_: Raw data size=2892 bytes, bitRate=2400000 bps
D (316) I2S_: Optimised buffer size=2892 bytes, frames/chunk=723, bytes/frame=4, DMA chunks=2
D (326) I2S_: Sample rate=75000 frames/sec
D (326) I2S_: Interrupt priority=0
I (326) I2S_: Started I2S channel=0, internal DMA buffer size=5784 bytes, required transmit time=19280 us
D (336) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (346) I2S_: maxSendMicros=192
D (366) I2S_: maxSendMicros=16718
I (366) MAIN: maxNrChunksSent=2
I (366) MAIN: minimumFreeStack=828
...
D (2686) I2S_: maxSendMicros=19341
I (5096) MAIN: minimumFreeStack=728
...
```
