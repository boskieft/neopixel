# How to build using ESP-IDF

Espressif's ESP-IDF can be used to build and test an ESP-IDF example from this folder.

## Software required

Virtual Studio Code (VSC) with the "ESP-IDF" extension version 6.1.0

(in fact ESP-IDF v5.5.5 or higher should work, but the commands and responses may be a bit different than shown here)

## Select an ESP-IDF example

Copy/Clone the files from an example (e.g. examples\ESP-IDF\Blink) into a local project folder, so for example: `<YourPath>\ESP-IDF-blink`

VSC -> File -> Open Folder...

* Browse and click [Select folder]

In VSC Explorer, the minimum file structure for an example should look similar to this:

```text
ESP-IDF-blink
|-- .vscode
|    |-- c_cpp_properties.json
|
|-- main
|    |-- blink_main.cpp
|    |-- CMakeLists.txt
|    |-- idf_component.yml
|
|-- CMakeLists.txt
```

### NOTE on red squiggles and warnings

Looking into the *.cpp file(s) you may see red squiggles under the #includes, complaining that these files cannot be opened.

You may also get a VSC popup [Scan for kits] to set the compiler.

You can ignore all this, it will be solved in the next step: Configure.

## NOTE on waiting(!)

Do WAIT until the message `Waiting for ESP-IDF to setup CMake...` has disappeared from the VSC blue bottom row.

Also do WAIT for **FULL COMPLETION** of the upcoming `ESP-IDF: Explorer` commands, before starting a new one!

## Summary of commands

Below is an overview of the `ESP-IDF: Explorer` commands to run for an initial build.

* `Set current ESP-IDF version` = v6.1.0
* `Set Flash Method` = UART
* `Set Espressif Device Target (IDF_TARGET)`, to select the new ESP32xx device
* `SDK Configuration Editor (menuconfig)`, to enable Debug logging
* `Build Project`
* `Select Port to Use (COM, tty, usbserial)` = detect, for automatic detection of the COMxx port
* `Build, Flash and Monitor`

The next paragraphs describe these commands in more detail.

## Configure

In left sidebar of VSC, select `ESP-IDF: Explorer`

ESP-IDF: Explorer -> `Set current ESP-IDF version`

* Select e.g. `Version: v6.1.0`
* (this will update .vscode/settings.json)

ESP-IDF: Explorer -> `Set Flash Method`

* Select `UART`
* (this will also update .vscode/settings.json)

ESP-IDF: Explorer -> `Set Espressif Device Target (IDF_TARGET)`

* Select your ESP32xx device from the list, e.g. `esp32c3`
* Next, you must specify what type exactly, e.g. `ESP32-C3 chip (via buildin USB-JTAG)`
* (once again this will update .vscode/settings.json)

In the right hand corner of the VSC screen (or in the VSC [Output] window) you can see the progress. After a short time, "(i) Target ESP32C3 Set Successfully." is shown.

### Added files after Configure

The next files and folders are **added** now:

```text
ESP-IDF-blink
|-- build
|-- managed components
|     |-- neopixel
|           |-- ...
|
|-- dependencies.lock
|-- sdkconfig
```

And the file `.vscode\settings.json` is updated to your local build environment.

## Optional: Enable Debug logging

By default, only Info messages and above are enabled by EDF-IDF. You can change this to Debug:

VSC, ESP-IDF: Explorer -> `ESP-IDF: SDK Configuration Editor (menuconfig)`

After some setup the `SDK Configuration editor` is opened in a tab.

In the index (left hand side) go to Log -> `Log Level`

On the right hand side change:

Log Level:

* Maximum log verbosity = `Debug`
* (do NOT change the _default_ verbosity)

Format:

* `[v]` Color

This will make the required (CONFIG_LOG_*) changes in the relevant `sdkconfig` file.

## Build

ESP-IDF: Explorer -> `Build Project`

In the VSC [Terminal] window the next text appears:

```code
...
Partition table binary generated. Contents:
*******************************************************************************
# ESP-IDF Partition Table
# Name, Type, SubType, Offset, Size, Flags
nvs,data,nvs,0x9000,24K,
phy_init,data,phy,0xf000,4K,
factory,app,factory,0x10000,1M,
*******************************************************************************
[xxx/yyy] compile and link files    <- gets updated very fast
```

Finally, you get a table with memory usage, and a popup "(i) Build Successful" in the lower part of the VSC screen.

## Select Port to Use (for USB-C)

ESP-IDF: Explorer -> `Select Port to Use (COM, tty, usbserial)`

* Select `detect - Auto-detect port (let esptool.py find the device automatically)`
* (this will update .vscode/settings.json)

VSC [Terminal] will show the progress:

```code
Found 7 serial ports...
Serial port COM10:
Connecting...
Connected to ESP32-C3 on COM10:
Chip type:          ESP32-C3 (QFN32) (revision v0.4)
Features:           Wi-Fi, BT 5 (LE), Single Core, 160MHz, Embedded Flash 4MB (XMC)
Crystal frequency:  40MHz
USB mode:           USB-Serial/JTAG
...
Hard resetting via RTS pin...
```

In the blue VSC bar on the bottom, the selected COM10 port is shown, next to a plug symbol.

## Build, Flash and Monitor

ESP-IDF: Explorer -> `Build, Flash and Monitor`

The software will be flashed to the ESP32xx device, a hard reset with the RTS pin will be done, and the VSC [Terminal] will show the logging:

```code
...
--- Warning: GDB cannot open serial ports accessed as COMx
--- Using \\.\COM10 instead...
--- esp-idf-monitor 1.9.0 on \\.\COM10 115200
...
rst:0x15 (USB_UART_CHIP_RESET),boot:0xf (SPI_FAST_FLASH_BOOT)
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

The `BLINK` log lines comes from the Blink example itself, more logging can follow.

## Optional: Switch to another ESP32xx device

To test on another ESP32 Target Device, in `ESP-IDF: Explorer` click on the next commands:

* `Set Espressif Device Target (IDF_TARGET)`, to select the new ESP32xx device
* `SDK Configuration Editor (menuconfig)`, to enable Debug logging again
* `Build Project`
* `Select Port to Use (COM, tty, usbserial)`, because the new device probably uses another COMxx port
* `Build, Flash and Monitor`

## Optional: Rebuild from scratch

Remove the files and folders that were added after Configure (see above, paragraph: `Added files after Configure`).

In short, remove:

* folder: `managed_components`
* file: `dependencies.lock`
* file: `sdkconfig`

ESP-IDF Explorer -> click on:

* `Fullclean`, to remove the `build` folder
* `SDK Configuration Editor (menuconfig)`, to enable Debug logging again
* `Build, Flash and Monitor`
