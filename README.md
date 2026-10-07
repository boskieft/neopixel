# Neopixel driver for ESP32xx using I2S

Reliable ESP32xx driver for Neopixels, using I2S with DMA.

This driver supports addressable RGB/RGBW LEDs using WS2812x / SK6812x compatible signaling, including Adafruit NeoPixel products and compatible LEDs from other manufacturers.

Features:

- **ESP32xx family:** Tested on 5 ESP32xx variants (see paragraph below).
- **ESP-IDF:** Works with and without the Arduino framework.
- **I2S:** Using standard I2s driver of ESP-IDF 5.5:
  - DMA for minimal processor load and interrupts
  - Keeps SPI with DMA available for other purposes
- **GRB/GRBW:** Configurable for GRB (3 colors) or GRBW (3 colors + white) Neopixels.
- **SEQ3/SEQ4:** Configurable for best match on your Neopixel's timing:
  - 3 bit sequence, period=1200ns, dutycycle: Off=33%, On=67%
  - 4 bit sequence, period=1200ns, dutycycle: Off=25%, On=50%
- **1...N:** Drives 1 up to many thousants of Neopixels, only limited by the available RAM.
- **Fast:** Driver waits for transmission completion in separate task.
- **Easy:** No polling, no throttling, the driver takes care.
- **Rotate:** In-memory left or right rotation
- **Fill:** In-memory color filling for a range of Neopixels (or all of them)
- **Logging:** Using standard ESP_LOGx(), activate Debug for more
- **Statistics:** On timing and errors

## Tested ESP32xx devices

Tested successfully on the ESP32xx devices listed below.

- ESP32 (D1 mini)
- ESP32-S2 (Wemos S2 Mini)
- ESP32-S3 (Lilygo T7)
- ESP32-C3 (Mini Pro)
- ESP32-C6 (Seeed)

## Speed and RAM memory

@@@TODO: describe typical show() response time
@@@TODO: describe typical overall transmission time (or repetition rate) for 1, 10, 100 and 1000 Neopixels

@@@TODO: describe buffer and DMA sizes for 1, 10, 100 and 1000 Neopixels

## Build environment

@@@TODO: briefly describe: ESP-IDF, pioarduino and Arduino

## Development (dev) folder

This repository is an ESP-IDF component (aka: library).

For development purposes, the `dev` folder contains a small ESP-IDF development project used to configure and test the component without changing the reusable component layout. The repository root `CMakeLists.txt` supports direct ESP-IDF commands from this workspace while still acting as a reusable component when included elsewhere. This way, the component can be easily be tested without having a separate test program.

See the README.md in the `dev` folder on how to use it.

Initially, VSC may show #include errors and missing settings in the .vscode files. These will be solved automatically while bulding the test program.

## Examples and Documentation

Please refer to the "examples" and "docs" folders in this driver.

## @@@TODO topics

Installation
Quick start
PixelType
RGB vs RGBW
SEQ3 vs SEQ4
Brightness
show() behavior
Memory usage
Supported chips
ESP-IDF version
pioArduino example
