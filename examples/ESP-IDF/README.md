# How to use ESP-IDF

Espressif's ESP-IDF can be used to build and test the driver itself, or a separate test program.

## Software required

Virtual Studio Code (VSC) with the "ESP-IDF" extension version 5.5.5 or higher (here version 6.1.0 was used).

## Open folder

Copy/Clone the files from an example (e.g. examples\ESP-IDF\Blink) into a project folder, so for example: `<YourPath>\ESP-IDF-blink`

VSC -> File -> Open Folder...

* Browse and use [Select folder]

In VSC Explorer, the minimum file structure for a `test program` should look similar to this

```text
ESP-IDF-blink
>-- main
|    |-- blink_example_main.cpp
|    |-- CMakeLists.txt
|    |-- idf_component.yml
|
|-- CMakeLists.txt
|-- README.md (this file)
```

## Configure

In left sidebar of VSC, select `ESP-IDF: Explorer`

ESP-IDF: Explorer -> `Set current ESP-IDF version`

* Select e.g. `Version: v6.1.0`

ESP-IDF: Explorer -> `Set Espressif Device Target (IDF_TARGET)`

* Select your ESP32xx device from the list, e.g. `esp32c3`
* Next, you must specify what type exactly, e.g. `ESP32-C3 chip (via buildin USB-JTAG)`

In the VSC [Output] window the next text appears:

```text
    Open On-Chip Debugger v0.12.0-esp32-20260703 (2026-07-03-13:40)
    ...
    [Set Target]
    Running IDF Set Target action
    ...
    -- Configuring done (17.9s)
    -- Generating done (1.1s)
    -- Build files have been written to: <YourPath>/ESP-IDF-blink/build

    Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
    Executing action: fullclean
    Executing action: set-target
    Set Target to: esp32c6, new sdkconfig will be created.
    Running cmake in directory <YourPath>\ESP-IDF-blink\build
    Executing "cmake -G Ninja -B <YourPath>\ESP-IDF-blink\build -DPYTHON_DEPS_CHECKED=1 -DPYTHON=C:\Espressif\tools\python\v6.1\venv\Scripts\python.exe -DESP_PLATFORM=1 -DIDF_TARGET=esp32c6 -DCCACHE_ENABLE=False <YourPath>\ESP-IDF-blink"...

    Target ESP32C6 Set Successfully.
```

### Added files after Configure

The next files and folders are **added** now:

```text
ESP-IDF-blink
|-- managed components
|     |-- neopixel
|           |-- ...
|
|-- dependencies.lock
|-- sdkconfig
```

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
[xxx/yyy] compile and link files
```

Finally, you get a table with memory usage.

## Select port for USB-C

ESP-IDF: Explorer -> `Select Port to Use (COM, tty, usbserial)`

* Select `detect - Auto-detect port (let esptool.py find the device automatically)

In the blue VSC bar on the bottom, the selected COMxx port is shown, next to a plug symbol.

## Build, Flash and Monitor

ESP-IDF: Explorer -> `Build, Flash and Monitor`

* Software will be flashed to the ESP32xx device
* Terminal will be started and show the logging

```code
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
```

From here, the logging is about the application.

## Rebuild from scratch

Remove the files and folders that were added after Confufure (see above, paragraph: `Added files after Configure`).

In short, remove:

* folder: `managed_components`
* file: `dependencies.lock`
* file: `sdkconfig`

ESP-IDF Explorer -> click on:

* Fullclean
* Build, Flash and Monitor
