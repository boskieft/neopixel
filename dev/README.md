# Development

This "neopixel" repository is an ESP-IDF component (aka: library), not a standalone ESP-IDF application.

The `dev/` directory (here) contains a small ESP-IDF test program, that can be used to compile and run the component on target.

## Make link

**\/!\\ ATTENTION:** First time after download or clone, you must make a link in the `dev\components` folder.

See instructions in README.md of that folder.

## Configure

ESP-IDF: Explorer -> `Set current ESP-IDF version`

        Select eg "Version: v6.1.0"

ESP-IDF: Explorer -> `Set Espressif Device Target (IDF_TARGET)`

        Select your ESP32xx device from the list, eg "esp32c3"

        Next, you must specify what type exactly, eg "ESP32-C3 chip (via buildin USB-JTAG)"

## Build and Run

ESP-IDF: Explorer -> `Build Project`

        While building, connect the ESPxx device with USB-C to your laptop

Blue VSC status bar on bottom -> COMxx (select Port to use)

        Select `detect` from dropdown in top of the screen, for automatic detection of the assigned COMxx port.

ESP-IDF: Explorer -> `Build, Flash and Monitor`

        Software will be flashed to the ESP32xx device

        Terminal will be started and show the logging

## @@@TODO: remove the rest?

View -> Command Palette -> "ESP-IDF Terminal":

You may first need to select the ESP-IDF version:

- Click on [Select ESP-IDF Version] right hand side corner
- On top bar, select for example "ESP-IDF 6.1"

\=\> Expected output:

        IDF PowerShell Environment
        -------------------------
        Environment variables set:
        IDF_PATH: C:\esp\v6.1\esp-idf
        IDF_TOOLS_PATH: C:\Espressif\tools
        IDF_PYTHON_ENV_PATH: C:\Espressif\tools\python\v6.1\venv

        Custom commands available:
        idf.py - Use this to run IDF commands (e.g., idf.py build)
        esptool.py
        espefuse.py
        espsecure.py
        otatool.py
        parttool.py

        Python environment activated.
        You can now use IDF commands and Python tools.

        (venv) PS C:\Users\Erik\VSCode_Projects\neopixel> 

Now enter in the (venv) Terminal:

        cd dev
        idf.py set-target esp32c3       (or or another ESP32xx chip you may have)
        idf.py reconfigure

\=\> Expected output:

        Adding "set-target"'s dependency "fullclean" to list of commands with default set of options.
        Executing action: fullclean
        Build directory '<YourPath>\neopixel\dev\build' is empty. Nothing to clean.
        Executing action: set-target
        Set Target to: esp32, new sdkconfig will be created.
        [...]
        -- Configuring done (31.0s)
        -- Generating done (2.0s)
        -- Build files have been written to: <YourPath>/neopixel/dev/build

The reconfigure step generates `dev/build/compile_commands.json`.

VS Code uses that compilation database for C/C++ IntelliSense, so ESP-IDF system headers and the target-specific compiler configuration are resolved from the actual ESP-IDF build rather than from hard-coded include paths.
