# How to build using ESP-IDF

Espressif's ESP-IDF can be used to build and test an ESP-IDF example from this folder.

## Software required

Virtual Studio Code (VSC) with the "ESP-IDF" extension version 5.5.5 or higher (here version 6.1.0 was used).

## Select an ESP-IDF example

Copy/Clone the files from an example (e.g. examples\ESP-IDF\Blink) into a local project folder, so for example: `<YourPath>\ESP-IDF-blink`

VSC -> File -> Open Folder...

* Browse and use [Select folder]

In VSC Explorer, the minimum file structure for an example should look similar to this

```text
ESP-IDF-blink
>-- main
|    |-- blink_example_main.cpp
|    |-- CMakeLists.txt
|    |-- idf_component.yml
|
|-- CMakeLists.txt
|-- README.md   (optional)
```

### NOTE on red squiggles and warnings

Looking into the *.cpp file(s) you may see red squiggles under the #includes, complaining that these files cannot be opened.

You may also get a VSC popup [Scan for kits] to set the compiler.

You can ignore all this, it will be solved in the next step: Configure.

## Configure

In left sidebar of VSC, select `ESP-IDF: Explorer`

ESP-IDF: Explorer -> `Set current ESP-IDF version`

* Select e.g. `Version: v6.1.0`
* (this will update .vscode/settings.json)

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
[xxx/yyy] compile and link files    <- gets udated very fast
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

The first time, Select Flash Method" is shown on the top the the VSC screen

* Select `UART`
* (this will update .vscode/settings.json)

The software will be flashed to the ESP32xx device, a hard reset with the RTS pin will be done, and the VSC [Terminal] will show the logging:

```code
I (24) boot: ESP-IDF v6.1 2nd stage bootloader
I (24) boot: compile time Oct  7 2026 11:20:06
I (25) boot: chip revision: v0.4
I (25) boot: efuse block revision: v1.3
I (28) boot.esp32c3: SPI Speed      : 80MHz
I (32) boot.esp32c3: SPI Mode       : DIO
...
I (155) app_init: Application information:
I (159) app_init: Project name:     ESP-IDF-blink
I (164) app_init: App version:      1
I (167) app_init: Compile time:     Oct  7 2026 11:19:52
...
I (253) main_task: Started on CPU0
I (253) main_task: Calling app_main()
I (253) BLINK: Status LED on GPIO=8
```

The last log line comes from the Blink example itself, more logging can follow.

## Rebuild from scratch

Remove the files and folders that were added after Configure (see above, paragraph: `Added files after Configure`). No need to remove the `build` folder, this will be done by Fullclean.

In short, remove:

* folder: `managed_components`
* file: `dependencies.lock`
* file: `sdkconfig`

ESP-IDF Explorer -> click on:

* Fullclean
* Build, Flash and Monitor
