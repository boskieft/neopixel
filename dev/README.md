# Development

This "neopixel" repository is an ESP-IDF component (aka: library), NOT a standalone ESP-IDF application.

However, the `dev` directory (here) also contains the ESP-IDF Test Program, that can be used to compile and run the component on target.

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

## GitHub Actions release procedue (recommended)

* In VSC Source Control, Commit and Push the changes to GitHub
* In GitHub tab `Actions`, run the next `Workflows`:
  * `Release Version` -> this will update the version in the next repo files, and tag the release:
    * include\neopixel_version.h (for driver itself)
    * idf_component.yml (manifest for ESP-IDF)
    * library.json (manifest for Platmormio)
  * `Publish ESP-IDF component`-> upload to the ESP Registry [https://components.espressif.com/components/boskieft/neopixel]
  * `Publish to Platformio` -> upload to the Platformio Registry: [https://registry.platformio.org/libraries/boskieft/neopixel]
* In VSC Source Control, Pull the changes from GitHub (to have the repo files with updated version info)

-/-/-/-/-/-/-/-

## Manual Release procedure (OBSOLETE)

### Manual ESP: Create release on GitHub

* Update `neopixel\idf_component.yml` with the new version (eg `version: "2.1.0"`)
* In VSC Source Control, Push the changes to GitHub
* In VSC Source Control, create a Tag (eg v2.0.1) and Push the tag to GitHub
* In GitHub, create a Release (eg v2.1.0) based on that tag

### Manual ESP: One time only: Login to registry

In Windows, open `Terminal` with PowerShell (PS prompt), and give command:

```code
compote registry login --default-namespace boskieft
```

The web brower will open a page and ley you create \<YOURTOKEN\> of almost 100 characters.

Copy/paste this token at the "Token:" prompt in `Terminal`:

```text
If browser did not open automatically please visit this URL:
        https://components.espressif.com/settings/...etc...
Please create a token in the browser window and paste here
Token: <YOURTOKEN>
Successfully logged in
```

### Manual ESP: Remove the link

Remove the link in the `dev\components` folder (see above) temporarily, to prevent an endless loop while collecting files.

#### Manual ESP: Upload

In Windows, open `Terminal` with PowerShell (PS prompt), and give command:

```code
compote component upload --name neopixel 
```

Example:

```text
PS <YOURPATH>\neopixel> compote component upload --name neopixel  
NOTE: Saving component archive to "<YOURPATH>\neopixel\dist\neopixel_2.0.0.tgz"
NOTE: Uploading archive <YOURPATH>\neopixel\dist\neopixel_2.0.0.tgz
Uploading ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100.0% 45.38kB/45.38kB [0s] 
NOTE: Wait for processing, it is safe to press CTRL+C and exit
You can check the state of processing by running CLI command "compote component upload-status --job=<JOBNUMBER> "
Processing examples ━━━━━━━━━━━━━━━━━━━━━━━━━━━╺━━  90.0% 90.0/100 [7s]
NOTE: The uploaded component was successfully processed. It may take up to 5 minutes for the new version to be available globally.
View at: https://components.espressif.com/components/boskieft/neopixel/versions/2.0.0
Processing examples ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100.0% 100/100 [10s] 
```

### Manual ESP: Recreate the link

Enable the development environment again, by recreating the link in folder `dev\components`.

See instructions in README.md of that folder.

### Manual Platformio

Never did this, used GitHub Actions from start
