# Neopixel driver for ESP32xx using I2S

ESP-IDF [![Component Registry](https://components.espressif.com/components/boskieft/neopixel/badge.svg)](https://components.espressif.com/components/boskieft/neopixel)
<!--[![PlatformIO Registry](https://badges.registry.platformio.org/packages/boskieft/library/neopixel.svg)](https://registry.platformio.org/libraries/boskieft/neopixel)-->
[![GitHub latest commit](https://badgen.net/github/last-commit/boskieft/neopixel)](https://GitHub.com/boskieft/neopixel/commit/)

Rock solid, low profile ESP32xx driver for Neopixels, using I2S with DMA.

This driver supports addressable RGB/RGBW LEDs using WS2812x / SK6812x compatible signaling, including Adafruit NeoPixel products and compatible LEDs from other manufacturers.

Features:

- **ESP32xx family:** Tested on 5 ESP32xx variants (see paragraph below).
- **ESP-IDF:** Works on ESP-IDF v5.5.5 and above, with and without the (Pio)Arduino framework.
- **I2S:** Using the standard I2S driver of ESP-IDF:
  - DMA for minimal processor load and interrupts.
  - Keeps SPI with DMA available for other purposes.
- **GRB/GRBW:** Configurable for GRB (3 colors) or GRBW (3 colors + white) Neopixels.
- **SEQ3/SEQ4:** Configurable for best match on your Neopixel's timing:
  - 3 bit sequence, period=1200ns, dutycycle: Off=33%, On=67%
  - 4 bit sequence, period=1200ns, dutycycle: Off=25%, On=50%
- **1...N:** Drives 1 up to more than 20000 Neopixels (limited by RAM memory and transmission time < 1 sec)
- **Fast:** Driver waits for transmission completion in separate task.
- **Easy:** No polling, no throttling, the driver takes care.
- **In-memory functions:** for filling and rotating, for low CPU impact
- **Logging:** Using standard ESP_LOGx().
- **Statistics:** On timing and errors.

## Tested ESP32xx devices

Tested successfully on the ESP32xx devices listed below.

- ESP32 (D1 mini)
- ESP32-S2 (Wemos S2 Mini)
- ESP32-S3 (Lilygo T7)
- ESP32-C3 (Mini Pro)
- ESP32-C6 (Seeed)

## Build environment

The driver is written in C++ using the ESP-IDF environment, tested with version 5.5.5 (minimum) and 6.1 (current). It has no dependencies towards pioarduino or Arduino, but can simply be used by them.

## Functions

```code
begin(nrPixels, dataPin);         // initialise the driver, allocates buffer
hasWhite();                       // returns true if the Neopixels have a separate white color LED
show();                           // send the buffer (result of called set*() and rotate*() functions) to the Neopixels

setPixel(index, color);           // set pixel[index] in the buffer to color
setAllPixels(color);              // set all pixels in the buffer to same color
setPixelRange(start, end, color); // set range[start,end] of pixels in the buffer to same color, do not change the others

rotateAllLeft();                  // rotate all pixels in the buffer to the Left (decreasing index)
rotateAllRight();                 // rotate all pixels in the buffer to the Right (increasing index)

rotateRangeLeft(start, end);      // rotate the range[start,end] of pixels in the buffer to the Left, do not change the others
rotateRangeRight(start, end);     // rotate the range[start,end] of pixels in the buffer to the Right, do not change the others
```

## PixelColor

The color of a pixel is stored in a `PixelColor` variable that contains the Red, Green, Blue and (optionally) White components.

The `makePixelColor` shorthand can be used to declare a color variable easily:

```code
PixelColor red = makePixelColor(0xff0000);      // 0x(ww)rrggbb order
PixelColor green = makePixelColor(0, 0xff, 0);  // R, G, B (,W) order
```

For changing an existing color, the color or value fields can be used:

```code
PixelColor myPixel = neopixelBlack;
myPixel.color.r = 0xff;     // just change the red channel
// or:
myPixel.value = 0xff0000;   // set all channels at once
```

## Code snippet

Below the code that is needed to set the first Neopixel to medium bright red, on a total set of 24 Neopixels

```code
NeopixelDriver<PixelType::GRB_SEQ3> npx;  // instantiate the driver for 3-cololor Neopixels, GRB transmission order, SEQ3 timing
npx.begin(24, GPIO_NUM_5);                // init for 24 Neopixels in total, ESP32xx output pin = 5
npx.brightness = 0x10;                    // medium brightness
npx.setPixel(0, neopixelRed);             // set first to Red
npx.show();                               // show it on the Neopixels
```

## Examples

The `examples` folder contains demos for the `ESP-IDF` and `pioarduino` environment.

Each environment contains a README.md file, explaning how to build in general.

### ESP-IDF

- Blink - blink one Neopixel
- Chaser - run Neopixels one afer another

More examples in `pioarduino`, you can adapt them yourself to run in the ESP-IDF environment.

### pioarduino

- Blink - blink one Neopixel
- Chaser - run Neopixels one afer another
- Rainbow - all colors distributed over the Neopixels, slowly turning by in-memory rotation
- Range - filling and rotating a part of the Neopixels only, using in-memory functions

## Documentation (docs) folder

The folder `docs` contains detailed information on I2S, the SEQ/SEQ4 timing, choosing the right PixelType and about the wiring.

## Development (dev) folder

This repository is an ESP-IDF **_component_** (aka: library).

Additionally, the `dev` folder contains the ESP-IDF environment to build and run an extensive Test Program for development purposes. This way, the component can easily be tested without the need of having a separate (example) program.

See the README.md in the `dev` folder on how to use it.
