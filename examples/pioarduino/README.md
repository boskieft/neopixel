# How to build using pioarduino

The Platformio variant `pioarduio` can be used to build and test an example from this folder.

NOTE: the original Platformio cannot be used with Arduino anymore, because Platformio ceased their support years ago. The old version is still available, but does not work with recent ESP-IDF v5 and up.

## Software required

Virtual Studio Code (VSC) with the `pioarduino` extension version 1.4.4 or above.

NOTE: when using the `pioarduino` extension, make sure you don't have the original `Platformio` installed as well, bacause this can cause weird problems.

## Select an ESP-IDF example

Copy/Clone the from an example (e.g. examples\pioarduino\Chaser) the next folder/files into a local project folder, eg: `<YourPath>\pio-chaser`

* folder: examples\pioarduino\Chaser\src -> \<YourPath\>\pio-chaser\src
* file: examples\pioarduino\set_gpio.h -> \<YourPath\>\pio-chaser\src
* file: examples\pioarduino\platformio.ini -> \<YourPath\>\pio-chaser

VSC -> File -> Open Folder...

* Browse and use [Select folder]

In VSC Explorer, the minimum file structure for an example should look similar to this:

```text
pio-chaser
|-- src
|    |-- chaser_main.cpp
|    |-- set_gpio.h   (optional)
|
|-- platformio.ini
```

## EPS32xx variants, GPIO numbers

In `Platformio.ini` my (Eriks) ESP32xx variants and their boards are defined. You can change this to your needs. Make sure the [env:esp32xx] lowercase names match with the CONFIG_IDF_TARGET_ESP32xx uppercase names in `set_gpio.h`.

The file `set_gpio.h` sets the correct GPIO pin numbers, based on the selected esp32xx chip in VSC, pioarduino -> Project Tasks. This is especially convenient when working with multiple ESP32xx variants.

NOTE: the GPIO pin(s) fully depend on the used ESP32xx chip and the hardware circuit around it. Most probably you have to change it to your situation. You can do this in `set_gpio.h`, or directly in the example *.cpp file (and comment-out the #include of `set_gpio.h`).

The `enablePin` is only needed in case your (74HCT126) level shifter has an Enable input, that needs to be driven by your ESP32xx chip. In case you do NOT have it, set `enablePin = GPIO_NUM_NC`.

## pioarduino version

The used pioarduino version is fixed in `platformio.ini`, for example:

```platform = https://github.com/pioarduino/platform-espressif32/releases/download/55.03.311/platform-espressif32.zip```

This version uses Arduino v3.3.11 based on ESP-IDFv5.5.5.

Pioarduino is updated on a regular base, see: <https://github.com/pioarduino/platform-espressif32/releases>

You can use a newer `pioarduino` version by updating the above line in `Platformio.ini`

## NOTE on red squiggles and warnings

Looking into the *.cpp file(s) you may see red squiggles under the #includes, complaining that these files cannot be opened.

You may also get a VSC popup [Scan for kits] to set the compiler.

You can ignore all this, it will be solved in the next step: Build.

## Build

In left sidebar of VSC, select `pioarduino`

Pioarduino -> `Project Tasks`

* Select your ESP32xx configuration, e.g. `esp32c3`
* (NOTE: as stated before, the ESP32xx variants are defined in Platformio.ini as `[env:esp32xx]`)

... -> `esp32c3`-> General -> Build

```code
...
PLATFORM: Espressif 32 (55.3.311) > Adafruit QT Py ESP32-C3
HARDWARE: ESP32C3 160MHz, 320KB RAM, 4MB Flash
...
Found 45 compatible libraries
Scanning dependencies...
Dependency Graph
|-- neopixel @ 0.0.0+20261009230123.sha.6636581
Building in release mode
Compiling .pio\build\esp32c3\libd34\neopixel\neopixel.cpp.o
Compiling .pio\build\esp32c3\libd34\neopixel\neopixel_i2s.cpp.o
Compiling .pio\build\esp32c3\src\main.cpp.o
Building .pio\build\esp32c3\bootloader.bin
...
Checking size .pio\build\esp32c3\firmware.elf
Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
RAM:   [          ]   4.4% (used 14572 bytes from 327680 bytes)
Flash: [==        ]  24.6% (used 322804 bytes from 1310720 bytes)
...
Successfully created combined binary image.
```

## Upload and Monitor

Pioarduino -> Project Tasks -> `esp32c3` -> General -> `Upload and Monitor`

The required COMxx port will be chosen automatically and the firmware will be flashed.

```code
Configuring upload protocol...
AVAILABLE: cmsis-dap, esp-bridge, esp-builtin, esp-prog, esp-prog-2, espota, esptool, iot-bus-jtag, jlink, minimodule, olimex-arm-usb-ocd, olimex-arm-usb-ocd-h, olimex-arm-usb-tiny-h, olimex-jtag-tiny, tumpa
CURRENT: upload_protocol = esptool
Looking for upload port...
Auto-detected: COM10
Uploading .pio\build\esp32c3\firmware.bin
esptool v5.3.0
Serial port COM10:
Connecting...
Connected to ESP32-C3 on COM10:
Chip type:          ESP32-C3 (QFN32) (revision v0.4)
Features:           Wi-Fi, BT 5 (LE), Single Core, 160MHz, Embedded Flash 4MB (XMC)
Crystal frequency:  40MHz
USB mode:           USB-Serial/JTAG
MAC:                d8:3b:da:0f:a5:bc

Uploading stub flasher...
Running stub flasher...
Stub flasher running.
Changing baud rate to 460800...
Changed.

Configuring flash size...
Auto-detected flash size: 4MB

Writing 'C:\Users\Erik\Desktop\chaser-pio\.pio\build\esp32c3\bootloader.bin' at 0x00000000...
SHA digest in image updated.
Flash will be erased from 0x00000000 to 0x00004fff...
Compressed 19536 bytes to 12719...

Writing at 0x00000000 [░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░]   0.0% 0/12719 bytes... 
...
Writing at 0x00061e20 [██████████████████████████████] 100.0% 182201/182201 bytes... 
Wrote 335392 bytes (182201 compressed) at 0x00010000 in 2.3 seconds (1189.3 kbit/s).
Verifying written data...
Hash of data verified.

Hard resetting via RTS pin...
```

Next, VSC will switch automatically to the [ESP Decoder] tab

## ESP Decoder

This tab shows the ESP32xx logging, first part comes from Arduino, then from your application.

NOTE: after `Upload and Monitor`, the [ESP Decoder] starts up a bit too slow, causing it to skip the first line. By clicking [Reset] you can start again, and this time you will see everything.

```code
ESP-ROM:esp32c3-api1-20210207
Build:Feb  7 2021
rst:0x15 (USB_UART_CHIP_RESET),boot:0xf (SPI_FAST_FLASH_BOOT)
...
=========== Before Setup Start ===========
Chip Info:
------------------------------------------
  Model             : ESP32-C3
  Package           : 0
  Revision          : 0.04
  ...
INTERNAL Memory Info:
  ...
Flash Info:
  ...
Partitions Info:
  ...
Software Info:
------------------------------------------
  Compile Date/Time : Oct  9 2026 21:58:45
  ESP-IDF Version   : v5.5.5
  Arduino Version   : 3.3.11
------------------------------------------
Board Info:
------------------------------------------
  Arduino Board     : Adafruit QT Py ESP32-C3
  Arduino Variant   : adafruit_qtpy_esp32c3
  Core Debug Level  : 5
  Arduino Runs Core : 0
  Arduino Events on : 0
  Arduino USB Mode  : 1
  CDC On Boot       : 1
============ Before Setup End ============
I (238) ARDUINO: Pin 18 already has type USB_DM (39) with bus 0x3fc90908
I (238) ARDUINO: Pin 19 already has type USB_DP (40) with bus 0x3fc90908
I (5242) CHASER: ----- Running setup, chip=`esp32c3` -----
I (5242) CHASER: Using dataPin=5
I (5242) CHASER: Switching On enablePin=10
I (5245) CHASER: Init the Neopixels on pin=5 with 24 pixels
D (5252) NPIX: GRB Neopixels, seq3 timing
D (5256) I2S_: Big-Endian buffer
D (5260) I2S_: Raw data size=216 bytes, bitRate=2400000 bps
D (5265) I2S_: Optimised buffer size=216 bytes, frames/chunk=54, bytes/frame=4, DMA chunks=2
D (5274) I2S_: Sample rate=75000 frames/sec
D (5279) I2S_: Interrupt priority=0
I (5282) I2S_: Started I2S channel=0, internal DMA buffer size=432 bytes, required transmit time=1440 us
D (5292) I2S_: Starting separate Task for Transmit Control on core=0, priority=2
D (5300) I2S_: maxSendMicros=138
=========== After Setup Start ============
INTERNAL Memory Info:
  ...
GPIO Info:
  ...
============ After Setup End =============
I (5331) CHASER: Start the Chaser animation

```
