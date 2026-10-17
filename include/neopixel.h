#pragma once
/*
***************************************************************************************************
    ESP32xx Neopixel Driver, header file

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <esp_idf_version.h>
#if (ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 5, 5))
#error "This driver requires ESP-IDF >= 5.5.5"
#endif

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "neopixel_i2s.h" // use the I2S implementation for Neopixel data transmission

/*
===================================================================================================
    Union PixelColor

    Can be used as one 32-bit value, or by addressing the four individual r, g, b, w bytes
===================================================================================================
*/
typedef union alignas(uint32_t) UnionPixelColor {
    uint32_t value; // set as 0x(ww)rrggbb. NOTE: this is regardless of GRB(W) ordering of the transmission to the Neopixels

    struct StructPixelColor { // set as .b, .g, .r (, .w) bytes, order CANNOT be changed (Big/Little-Endian issue)
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t w; // white, only used for RGBW Neopixels with separate white LED, ignored for RGB Neopixels
    } color;
} PixelColor;

/*
===================================================================================================
    Shorthands for using PixelColor

    To avoid the somewhat complex syntax of addressing the PixelColor structure directly
===================================================================================================
*/
//-----------------------------------------------------------------------------
//  PixelColor myColor = makePixelColor(0xff, 0, 0); // R, G, B (W) order
//
//  is the short hand for:
//  PixelColor myColor = {.color = {.b = 0, .g = 0, .r = 0xff, .w = 0}};
//-----------------------------------------------------------------------------
__attribute__((always_inline)) static inline constexpr PixelColor makePixelColor(uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) {
    return PixelColor{
        .color = {.b = b, .g = g, .r = r, .w = w}};
}

//-----------------------------------------------------------------------------
//  PixelColor myColor = makePixelColor(0x00100000)  // (W)RGB order
//
//  is the short hand for:
//  PixelColor myColor = {.value = 0x00100000};
//-----------------------------------------------------------------------------
__attribute__((always_inline)) static inline constexpr PixelColor makePixelColor(uint32_t colorCode) {
    return PixelColor{.value = colorCode};
}

//-----------------------------------------------------------------------------
//  if (myColorA == myColorB) {}
//
//  is the shorthand for:
//  if (myColorA.value == myColorB.value) {}
//-----------------------------------------------------------------------------
__attribute__((always_inline)) static inline constexpr bool operator==(PixelColor a, PixelColor b) {
    return a.value == b.value;
}

//-----------------------------------------------------------------------------
//  if (isNeopixelBlack(myColor)) {}
//
//  is the shorthand for:
//  if (myColor.value == 0) {}
//-----------------------------------------------------------------------------
__attribute__((always_inline)) static inline constexpr bool isNeopixelBlack(PixelColor c) {
    return c.value == 0;
}

/*
===================================================================================================
    Primary colors at full brightness
===================================================================================================
*/
inline constexpr PixelColor neopixelBlack = makePixelColor(0);
inline constexpr PixelColor neopixelWhite_RGB = makePixelColor(0xff, 0xff, 0xff);
inline constexpr PixelColor neopixelWhite_RGBW = makePixelColor(0, 0, 0, 0xff);
inline constexpr PixelColor neopixelRed = makePixelColor(0xff, 0, 0);
inline constexpr PixelColor neopixelGreen = makePixelColor(0, 0xff, 0);
inline constexpr PixelColor neopixelBlue = makePixelColor(0, 0, 0xff);
inline constexpr PixelColor neopixelCyan = makePixelColor(0, 0xff, 0xff);
inline constexpr PixelColor neopixelMagenta = makePixelColor(0xff, 0, 0xff);
inline constexpr PixelColor neopixelYellow = makePixelColor(0xff, 0xff, 0);

/*
===================================================================================================
    Neopixel communication methods

    NOTE: see also explicit template instantiations in .cpp file

    seq3 encoding results in less RAM usage (smaller DMA chunks in I2S driver),
    while seq4 matches the timing requirements of most modern Neopixels better.
===================================================================================================
*/
enum class PixelType {
    GRB_SEQ3,  // GRB colors, using seq3 encoding (1/3 and 2/3 duty cycle)
    GRB_SEQ4,  // GRB colors, using seq4 encoding (1/4 and 2/4 duty cycle)
    GRBW_SEQ3, // GRBW colors, using seq3 encoding (1/3 and 2/3 duty cycle)
    GRBW_SEQ4  // GRBW colors, using seq4 encoding (1/4 and 2/4 duty cycle)
};

/*
===================================================================================================
    Neopixel Driver class

    Instantiate with a specific PixelType, for example:
    npx = NeopixelDriver<PixelType::GRB_SEQ3>;
===================================================================================================
*/
template <PixelType Mode>
class NeopixelDriver {
  private:
    // Neopixel config
    size_t colorsPerPixel;  // number of color components per pixel (3 for GRB, 4 for GRBW)
    size_t txBytesPerColor; // number of bytes to be sent per R/G/B/(W) color component, depends on seq3/seq4 timing
    size_t txBytesPerPixel; // number of bytes per Neopixel (all colors)

    // Data size
    size_t nrPixels = 0;       // number of Neopixels to drive
    uint8_t *buffer = nullptr; // data buffer to be sent to the Neopixels
    size_t bufferSize = 0;     // [bytes]

    // Transmission tracking
    NeopixelTransmitControl txControl; // class to control Neopixel data transmission, initialized with the stats pointer in the constructor

    // Private helper to fill a range of pixels with the specified color
    bool fillPixelRange_(size_t startIndex, size_t nrPixelsInRange, const PixelColor color);

  public:
    uint8_t brightness = 255;                     // global brightness (min=0...max=255)
    struct NeopixelTransmitStatistics stats = {}; // statistics for Neopixel data transmission

    //-------------------------------------------
    //  (De)Constructors
    //-------------------------------------------
    NeopixelDriver(void) : txControl(&stats) {} // initialize txControl with pointer to the statistics structure here

    // empty, use begin() to initialize the driver

    ~NeopixelDriver(void) {
        if (buffer != nullptr) {
            free(buffer);
            buffer = nullptr;
        }
        bufferSize = 0;
        nrPixels = 0;
    }

    // Delete copy and move constructors and assignment operators, to prevent copying/moving the driver instance
    NeopixelDriver(const NeopixelDriver &) = delete;
    NeopixelDriver &operator=(const NeopixelDriver &) = delete;

    NeopixelDriver(NeopixelDriver &&) = delete;
    NeopixelDriver &operator=(NeopixelDriver &&) = delete;

    //-------------------------------------------
    //  Basic functions
    //-------------------------------------------
    bool begin(const size_t nrPixels, const gpio_num_t dataPin); // in cpp, will allocate buffer

    bool hasWhite(void) const {
        return (colorsPerPixel == 4);
    }

    bool show(void) {
        return (txControl.startTransmit(buffer, bufferSize)); // transmit the buffer to the Neopixels
    }

    //-------------------------------------------
    //  Set pixel(s)
    //
    //  Returns false if index is out of range,
    //  true otherwise
    //-------------------------------------------
    bool setPixel(const size_t index, const PixelColor color);
    bool setAllPixels(const PixelColor color);
    bool setPixelRange(size_t startIndex, size_t endIndex, const PixelColor color);

    //-------------------------------------------
    //  Rotate pixels
    //
    //  Returns false if index is out of range
    //  OR: Neopixels are not rotatable,
    //  true otherwise
    //-------------------------------------------
    bool isRotatable(void) const {
        return ((nrPixels > 1) &&                       // useless to rotate if only one pixel
                ((NEOPIXEL_USE_BIG_ENDIAN_DATA == 1) || // always works with Big-Endian data
                 (txBytesPerPixel % 2 == 0))            // with Little-Endian data (ie older ESP32 variant), only works if txBytesPerPixel is even
        );
    }

    bool rotateAllLeft(void);
    bool rotateAllRight(void);

    bool rotateRangeLeft(size_t startIndex, size_t endIndex);
    bool rotateRangeRight(size_t startIndex, size_t endIndex);
};
