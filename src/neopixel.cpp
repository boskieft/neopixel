/*
***************************************************************************************************
    ESP32xx Neopixel Driver

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include <esp_system.h>
#include <esp_log.h>

#include "neopixel.h"
#include "neopixel_seq3.h"
#include "neopixel_seq4.h"

#define TAG "NPIX"

// Explicit template instantiation for all supported PixelType variants
// (see "enum class PixelType" in .h file for definition of PixelType)
template class NeopixelDriver<PixelType::GRB_SEQ3>;
template class NeopixelDriver<PixelType::GRB_SEQ4>;
template class NeopixelDriver<PixelType::GRBW_SEQ3>;
template class NeopixelDriver<PixelType::GRBW_SEQ4>;

/*
===================================================================================================
    Init the driver
===================================================================================================
*/
template <PixelType Mode>
bool NeopixelDriver<Mode>::begin(
    const size_t arg_nrPixels,  // number of Neopixels to drive
    const gpio_num_t dataPin) { // GPIO output pin for the Neopixel data

    uint32_t bitRate;

    if (arg_nrPixels == 0) {
        ESP_LOGE(TAG, "Number of pixels must be greater than zero");
        return (false);
    }

    if (buffer != nullptr) {
        ESP_LOGE(TAG, "Already initialised");
        return (false);
    }

    if constexpr (Mode == PixelType::GRB_SEQ3) {
        //---------------------------------------
        //  GRB, seq3 timing
        //---------------------------------------
        ESP_LOGD(TAG, "GRB Neopixels, seq3 timing");
        colorsPerPixel = 3;                              // GRB = 3 color components per pixel
        txBytesPerColor = NEOPIXEL_SEQ3_BYTES_PER_COLOR; // seq3 encoding uses 3 bits per color bit, so 3 bytes per R/G/B color component
        txBytesPerPixel = txBytesPerColor * 3;           // 3 color components (R, G, B), 9 bytes in total
        bitRate = (800000UL * txBytesPerColor);          // Neopixel at 800kHz * 3 bits = 2.4 Mbps (417 ns/bit)
    } else if constexpr (Mode == PixelType::GRB_SEQ4) {
        //---------------------------------------
        //  GRB, seq4 timing
        //---------------------------------------
        ESP_LOGD(TAG, "GRB Neopixels, seq4 timing");
        colorsPerPixel = 3;                              // GRB = 3 color components per pixel
        txBytesPerColor = NEOPIXEL_SEQ4_BYTES_PER_COLOR; // seq4 encoding uses 4 bits per color bit, so 4 bytes per R/G/B color component
        txBytesPerPixel = txBytesPerColor * 3;           // 3 color components (G, R, B), 12 bytes in total
        bitRate = (800000UL * txBytesPerColor);          // Neopixel at 800kHz * 4 bits = 3.2 Mbps (312.5 ns/bit)
    } else if constexpr (Mode == PixelType::GRBW_SEQ3) {
        //---------------------------------------
        //  GRBW, seq3 timing
        //---------------------------------------
        ESP_LOGD(TAG, "GRBW Neopixels, seq3 timing");
        colorsPerPixel = 4;                              // GRBW = 4 color components per pixel
        txBytesPerColor = NEOPIXEL_SEQ3_BYTES_PER_COLOR; // seq3 encoding uses 3 bits per color bit, so 3 bytes per R/G/B/W color component
        txBytesPerPixel = txBytesPerColor * 4;           // 4 color components (R, G, B, W), 12 bytes in total
        bitRate = (800000UL * txBytesPerColor);          // Neopixel at 800kHz * 3 bits = 2.4 Mbps (417 ns/bit)
    } else if constexpr (Mode == PixelType::GRBW_SEQ4) {
        //---------------------------------------
        //  GRBW, seq4 timing
        //---------------------------------------
        ESP_LOGD(TAG, "GRBW Neopixels, seq4 timing");
        colorsPerPixel = 4;                              // GRBW = 4 color components per pixel
        txBytesPerColor = NEOPIXEL_SEQ4_BYTES_PER_COLOR; // seq4 encoding uses 4 bits per color bit, so 4 bytes per R/G/B/W color component
        txBytesPerPixel = txBytesPerColor * 4;           // 4 color components (G, R, B, W), 16 bytes in total
        bitRate = (800000UL * txBytesPerColor);          // Neopixel at 800kHz * 4 bits = 3.2 Mbps (312.5 ns/bit)
    } else {
        ESP_LOGE(TAG, "Unknown pixel type=%d", static_cast<int>(Mode));
        return (false);
    }

    //---------------------------------------
    //  Init the Transmit Control
    //---------------------------------------
    if (!txControl.init(dataPin, bitRate, /*raw data size=*/(arg_nrPixels * txBytesPerPixel), &bufferSize)) {
        ESP_LOGE(TAG, "Failed to initialize Transmit Control");
        return (false);
    }

    //---------------------------------------
    //  Allocate the data buffer
    //  for the Neopixel transmission
    //---------------------------------------
    buffer = (uint8_t *)malloc(bufferSize);
    if (buffer == nullptr) {
        ESP_LOGE(TAG, "Failed to allocate buffer of size %d bytes", bufferSize);
        txControl.deinit(); // cleanup the Task
        return (false);
    }
    memset(buffer, 0, bufferSize); // esp. to ensure the unused bytes in last frame are zeroed

    // Only now store the nrPixels
    // (when it remains 0, it means begin() was not called successfully and setPixel() calls will fail silently)
    nrPixels = arg_nrPixels;
    return (true);
}

/*
===================================================================================================
    Generic function to set the pulse transmit sequence of one single pixel in the buffer
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::setPixel(
    const size_t index, // index of the pixel to set, [0]=first
    PixelColor pixel) { // new color of the pixel

    if (index >= nrPixels) {
        return; // silently ignore
    }

    if (brightness != 255) {
        //---------------------------------------
        //  Apply global brighness
        //---------------------------------------
        if (pixel.color.r)
            pixel.color.r = (pixel.color.r * brightness) >> 8;
        if (pixel.color.g)
            pixel.color.g = (pixel.color.g * brightness) >> 8;
        if (pixel.color.b)
            pixel.color.b = (pixel.color.b * brightness) >> 8;
        if constexpr ((Mode == PixelType::GRBW_SEQ3) || (Mode == PixelType::GRBW_SEQ4)) {
            // Only when the Neopixels actually have a white component
            if (pixel.color.w)
                pixel.color.w = (pixel.color.w * brightness) >> 8;
        }
    } // else: max brightness, no adjustment

    if constexpr (Mode == PixelType::GRB_SEQ3) {
        //---------------------------------------
        //  Set one GRB pixel, seq3 timing
        //---------------------------------------
        size_t offset = index * txBytesPerPixel;

        const uint8_t *gSeq = neopixel_seq3_color_map[pixel.color.g];
        const uint8_t *rSeq = neopixel_seq3_color_map[pixel.color.r];
        const uint8_t *bSeq = neopixel_seq3_color_map[pixel.color.b];

#if (NEOPIXEL_USE_BIG_ENDIAN_DATA)
        // Big Endian: Simply a sequential write order in GRB format
        buffer[offset++] = gSeq[0];
        buffer[offset++] = gSeq[1];
        buffer[offset++] = gSeq[2];
        buffer[offset++] = rSeq[0];
        buffer[offset++] = rSeq[1];
        buffer[offset++] = rSeq[2];
        buffer[offset++] = bSeq[0];
        buffer[offset++] = bSeq[1];
        buffer[offset++] = bSeq[2];
#else
        // Little Endian: I2S hardware expects 16-bit integers with high and low bytes to be swapped, so do that here.
        // NOTE: due to this swapping, the spanned write index in the buffer is 1 more than the number of bytes per pixel.
        // Because the buffer size is rounded up to a multiple 4 bytes, the extra byte does not cause issues.
        buffer[(offset++) ^ 1] = gSeq[0];
        buffer[(offset++) ^ 1] = gSeq[1];
        buffer[(offset++) ^ 1] = gSeq[2];
        buffer[(offset++) ^ 1] = rSeq[0];
        buffer[(offset++) ^ 1] = rSeq[1];
        buffer[(offset++) ^ 1] = rSeq[2];
        buffer[(offset++) ^ 1] = bSeq[0];
        buffer[(offset++) ^ 1] = bSeq[1];
        buffer[(offset++) ^ 1] = bSeq[2];
#endif
    } else if constexpr (Mode == PixelType::GRBW_SEQ3) {
        //---------------------------------------
        //  Set one GRBW pixel, seq3 timing
        //---------------------------------------
        size_t offset = index * txBytesPerPixel;

        const uint8_t *gSeq = neopixel_seq3_color_map[pixel.color.g];
        const uint8_t *rSeq = neopixel_seq3_color_map[pixel.color.r];
        const uint8_t *bSeq = neopixel_seq3_color_map[pixel.color.b];
        const uint8_t *wSeq = neopixel_seq3_color_map[pixel.color.w];

#if (NEOPIXEL_USE_BIG_ENDIAN_DATA)
        // Big Endian: Simply a sequential write order in GRBW format
        buffer[offset++] = gSeq[0];
        buffer[offset++] = gSeq[1];
        buffer[offset++] = gSeq[2];
        buffer[offset++] = rSeq[0];
        buffer[offset++] = rSeq[1];
        buffer[offset++] = rSeq[2];
        buffer[offset++] = bSeq[0];
        buffer[offset++] = bSeq[1];
        buffer[offset++] = bSeq[2];
        buffer[offset++] = wSeq[0];
        buffer[offset++] = wSeq[1];
        buffer[offset++] = wSeq[2];
#else
        // Little Endian: I2S hardware expects 16-bit integers with high and low bytes to be swapped, so do that here.
        buffer[(offset++) ^ 1] = gSeq[0];
        buffer[(offset++) ^ 1] = gSeq[1];
        buffer[(offset++) ^ 1] = gSeq[2];
        buffer[(offset++) ^ 1] = rSeq[0];
        buffer[(offset++) ^ 1] = rSeq[1];
        buffer[(offset++) ^ 1] = rSeq[2];
        buffer[(offset++) ^ 1] = bSeq[0];
        buffer[(offset++) ^ 1] = bSeq[1];
        buffer[(offset++) ^ 1] = bSeq[2];
        buffer[(offset++) ^ 1] = wSeq[0];
        buffer[(offset++) ^ 1] = wSeq[1];
        buffer[(offset++) ^ 1] = wSeq[2];
#endif
    } else if constexpr (Mode == PixelType::GRB_SEQ4) {
        //---------------------------------------
        //  Set one GRB pixel, seq4 timing
        //
        //  Not sensitive to Little/Big-Endian
        //---------------------------------------
        auto set_seq4_color = [](uint8_t colorByte, uint8_t *buffer) { // lambda to set one color byte in seq4 format
            const uint8_t *hi = encode_seq4_nibble[colorByte >> 4];
            const uint8_t *lo = encode_seq4_nibble[colorByte & 0x0F];
            buffer[0] = hi[0];
            buffer[1] = hi[1];
            buffer[2] = lo[0];
            buffer[3] = lo[1];
        };

        size_t offset = index * txBytesPerPixel;
        set_seq4_color(pixel.color.g, &buffer[offset]);
        set_seq4_color(pixel.color.r, &buffer[offset + NEOPIXEL_SEQ4_BYTES_PER_COLOR]);
        set_seq4_color(pixel.color.b, &buffer[offset + (2 * NEOPIXEL_SEQ4_BYTES_PER_COLOR)]);
    } else if constexpr (Mode == PixelType::GRBW_SEQ4) {
        //---------------------------------------
        //  Set one GRBW pixel, seq4 timing
        //
        //  Not sensitive to Little/Big-Endian
        //---------------------------------------
        auto set_seq4_color = [](uint8_t colorByte, uint8_t *buffer) { // lambda to set one color byte in seq4 format
            const uint8_t *hi = encode_seq4_nibble[colorByte >> 4];
            const uint8_t *lo = encode_seq4_nibble[colorByte & 0x0F];
            buffer[0] = hi[0];
            buffer[1] = hi[1];
            buffer[2] = lo[0];
            buffer[3] = lo[1];
        };

        size_t offset = index * txBytesPerPixel;
        set_seq4_color(pixel.color.g, &buffer[offset]);
        set_seq4_color(pixel.color.r, &buffer[offset + NEOPIXEL_SEQ4_BYTES_PER_COLOR]);
        set_seq4_color(pixel.color.b, &buffer[offset + (2 * NEOPIXEL_SEQ4_BYTES_PER_COLOR)]);
        set_seq4_color(pixel.color.w, &buffer[offset + (3 * NEOPIXEL_SEQ4_BYTES_PER_COLOR)]);
    } // else: unknown pixel type, should not occur
}

/*
---------------------------------------------------------------------------------------------------
    Private Helper: Set a range of pixels in the buffer to the same color,
    starting     from startIndex and covering nrPixelsInRange pixels.
---------------------------------------------------------------------------------------------------
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::_fillPixelRange(size_t startIndex, size_t nrPixelsInRange, const PixelColor color) {
    // First use setPixel for two (!) pixels,
    // to avoid issue with odd number of bytesPerPixel and Little-Endian byte order
    setPixel(startIndex, color);
    if (nrPixelsInRange > 1) {
        setPixel(startIndex + 1, color);

        // Next duplicate to fill the rest of the range efficiently
        size_t copiedBytes = txBytesPerPixel * 2;
        size_t restBytes = (nrPixelsInRange * txBytesPerPixel) - copiedBytes;
        auto startBufferRange = &buffer[startIndex * txBytesPerPixel];

        while (restBytes >= copiedBytes) {
            memcpy(&startBufferRange[copiedBytes], startBufferRange, copiedBytes);
            restBytes -= copiedBytes;
            copiedBytes *= 2;
        }

        // Finally copy any remaining bytes that didn't fit into the doubling loop
        if (restBytes > 0) {
            memcpy(&startBufferRange[copiedBytes], startBufferRange, restBytes);
        }
    } // else: just one (1) Neopixel in the range
}

/*
===================================================================================================
    Set all pixels in the buffer to the same color
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::setAllPixels(const PixelColor color) {
    _fillPixelRange(0, nrPixels, color);
}

/*
===================================================================================================
    Set a range of pixels in the buffer to the same color

    startIndex and endIndex are inclusive.
    If startIndex > endIndex, the range wraps around the buffer.
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::setPixelRange(size_t startIndex, size_t endIndex, const PixelColor color) {
    // Protect against invalid indices
    if (startIndex >= nrPixels || endIndex >= nrPixels) {
        return;
    }

    if (startIndex <= endIndex) {
        // Fill the normal range (inclusive)
        _fillPixelRange(startIndex, endIndex - startIndex + 1, color);
    } else {
        // Wrap around: first fill from startIndex to the last Neopixel
        _fillPixelRange(startIndex, nrPixels - startIndex, color);

        // Next fill from the first Neopixel to endIndex (inclusive)
        _fillPixelRange(0, endIndex + 1, color);
    }
}

/*
===================================================================================================
    Rotate all pixels in the buffer
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::rotateAllLeft(void) {
    if (!isRotatable()) {
        return; // just do nothing
    }

    // Remember first
    uint8_t copiedBytes[txBytesPerPixel]; // Variable Length Array (VLA), not standard C++, but supported by GCC
    memcpy(copiedBytes, &buffer[0], txBytesPerPixel);
    // Move rest to left (use memmove to handle overlapping memory)
    memmove(&buffer[0], &buffer[txBytesPerPixel], txBytesPerPixel * (nrPixels - 1));
    // Put first at the end
    memcpy(&buffer[(nrPixels - 1) * txBytesPerPixel], copiedBytes, txBytesPerPixel);
}

template <PixelType Mode>
void NeopixelDriver<Mode>::rotateAllRight(void) {
    if (!isRotatable()) {
        return; // just do nothing
    }

    // Remember last
    uint8_t copiedBytes[txBytesPerPixel]; // Variable Length Array (VLA), not standard C++, but supported by GCC
    memcpy(copiedBytes, &buffer[(nrPixels - 1) * txBytesPerPixel], txBytesPerPixel);
    // Move rest to right (use memmove to handle overlapping memory)
    memmove(&buffer[txBytesPerPixel], &buffer[0], txBytesPerPixel * (nrPixels - 1));
    // Put last as first
    memcpy(&buffer[0], copiedBytes, txBytesPerPixel);
}

/*
===================================================================================================
    Rotate a range of pixels in the buffer - to the LEFT
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::rotateRangeLeft(size_t startIndex, size_t endIndex) {
    if (!isRotatable() || nrPixels < 2 ||
        startIndex >= nrPixels || endIndex >= nrPixels ||
        startIndex == endIndex) {
        return;
    }

    if (startIndex < endIndex) {
        //-------------------------------------------------------
        //  Normal range: just rotate Left by one pixel
        //-------------------------------------------------------
        // Remember the bytes of pixel[startIndex]
        uint8_t startPixelBytes[txBytesPerPixel];
        memcpy(startPixelBytes, &buffer[startIndex * txBytesPerPixel], txBytesPerPixel);

        // Move the range to the Left
        memmove(&buffer[startIndex * txBytesPerPixel],
                &buffer[(startIndex + 1) * txBytesPerPixel],
                (endIndex - startIndex) * txBytesPerPixel);

        // Place the remembered bytes of pixel[startIndex] at pixel[endIndex]
        memcpy(&buffer[endIndex * txBytesPerPixel], startPixelBytes, txBytesPerPixel);
    } else {
        //-------------------------------------------------------
        //  Range that wraps around the end of the buffer
        //  Rotate the Far Part and the Near Part independently
        //-------------------------------------------------------
        // Remember the bytes of pixel[startIndex], before it gets overwritten by Far Part
        uint8_t startPixelBytes[txBytesPerPixel];
        memcpy(startPixelBytes, &buffer[startIndex * txBytesPerPixel], txBytesPerPixel);

        // Remember the bytes of pixel[0], before it gets overwritten by the Near Part
        uint8_t firstPixelBytes[txBytesPerPixel];
        memcpy(firstPixelBytes, &buffer[0], txBytesPerPixel);

        // Move the Far Part of the range to the Left
        if (startIndex < nrPixels - 1) {
            memmove(&buffer[startIndex * txBytesPerPixel],
                    &buffer[(startIndex + 1) * txBytesPerPixel],
                    (nrPixels - startIndex - 1) * txBytesPerPixel);
        }

        // Move the Near Part of the range to the Left
        if (endIndex > 0) {
            memmove(&buffer[0],
                    &buffer[txBytesPerPixel],
                    endIndex * txBytesPerPixel);
        }

        // Place the remembered boundary pixels.
        memcpy(&buffer[(nrPixels - 1) * txBytesPerPixel], firstPixelBytes, txBytesPerPixel);
        memcpy(&buffer[endIndex * txBytesPerPixel], startPixelBytes, txBytesPerPixel);
    }
}

/*
===================================================================================================
    Rotate a range of pixels in the buffer - to the RIGHT
===================================================================================================
*/
template <PixelType Mode>
void NeopixelDriver<Mode>::rotateRangeRight(size_t startIndex, size_t endIndex) {
    if (!isRotatable() || nrPixels < 2 ||
        startIndex >= nrPixels || endIndex >= nrPixels ||
        startIndex == endIndex) {
        return;
    }

    if (startIndex < endIndex) {
        //-------------------------------------------------------
        //  Normal range: just rotate Right by one pixel
        //-------------------------------------------------------
        // Remember the bytes of pixel[endIndex]
        uint8_t endPixelBytes[txBytesPerPixel];
        memcpy(endPixelBytes, &buffer[endIndex * txBytesPerPixel], txBytesPerPixel);

        // Move the range to the Right
        memmove(&buffer[(startIndex + 1) * txBytesPerPixel],
                &buffer[startIndex * txBytesPerPixel],
                (endIndex - startIndex) * txBytesPerPixel);

        // Place the remembered bytes of pixel[endIndex] at pixel[startIndex]
        memcpy(&buffer[startIndex * txBytesPerPixel], endPixelBytes, txBytesPerPixel);
    } else {
        //-------------------------------------------------------
        //  Range that wraps around the end of the buffer
        //  Rotate the Far Part and the Near Part independently
        //-------------------------------------------------------
        // Remember the bytes of pixel[endIndex]
        uint8_t endPixelBytes[txBytesPerPixel];
        memcpy(endPixelBytes, &buffer[endIndex * txBytesPerPixel], txBytesPerPixel);

        if (endIndex > 0) {
            // Move the Near Part of the range to the right
            memmove(&buffer[txBytesPerPixel],
                    &buffer[0],
                    endIndex * txBytesPerPixel);
        }

        // Move the last pixel of the buffer to index 0.
        memcpy(&buffer[0],
               &buffer[(nrPixels - 1) * txBytesPerPixel],
               txBytesPerPixel);

        // Move the Far Part of the range to the right
        if (startIndex < nrPixels - 1) {
            memmove(&buffer[(startIndex + 1) * txBytesPerPixel],
                    &buffer[startIndex * txBytesPerPixel],
                    (nrPixels - startIndex - 1) * txBytesPerPixel);
        }

        // pixel[startIndex] = remembered pixel[endIndex]
        memcpy(&buffer[startIndex * txBytesPerPixel], endPixelBytes,
               txBytesPerPixel);
    }
}
