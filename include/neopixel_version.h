#pragma once
/*
***************************************************************************************************
    ESP32xx Neopixel Driver, version file

    Supposed to be updated by GitHub Actions with every release

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/

// Major version number (X.x.x)
#define NEOPIXEL_VERSION_MAJOR 2
// Minor version number (x.X.x)
#define NEOPIXEL_VERSION_MINOR 3
// Patch version number (x.x.X)
#define NEOPIXEL_VERSION_PATCH 1

// Macro to convert version number into an integer
// To be used in comparisons, such as NEOPIXEL_VERSION >= NEOPIXEL_VERSION_VAL(2, 0, 0)
#define NEOPIXEL_VERSION_VAL(major, minor, patch) ((major << 16) | (minor << 8) | (patch))

// Current version, as an integer
// To be used in comparisons, such as NEOPIXEL_VERSION_NUM >= NEOPIXEL_VERSION_VAL(2, 0, 0)
#define NEOPIXEL_VERSION_NUM NEOPIXEL_VERSION_VAL(NEOPIXEL_VERSION_MAJOR, NEOPIXEL_VERSION_MINOR, NEOPIXEL_VERSION_PATCH)

// Current version, as string
#define df2xstr(s) #s
#define df2str(s) df2xstr(s)
#define NEOPIXEL_VERSION df2str(NEOPIXEL_VERSION_MAJOR) "." df2str(NEOPIXEL_VERSION_MINOR) "." df2str(NEOPIXEL_VERSION_PATCH)
