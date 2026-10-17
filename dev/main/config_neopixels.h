#pragma once
/*
***************************************************************************************************
    Set the Neopixel Driver type and number of Neopixels

    Adjust as needed to your specific hardware setup.

    Copyright (c) 2026 Erik Boskieft. All rights reserved.
    Released under the MIT License, see the LICENSE file for details.
***************************************************************************************************
*/
#include "neopixel.h"

//------------------------------------------------
// Your configuration for Neopixel Driver
//------------------------------------------------
#if (0)
// 1 assemby of 9 rings with GRBW NeoPixels
using MyPixelDriver = NeopixelDriver<PixelType::GRBW_SEQ3>;
static const size_t nrNeopixels = (1 + 8 + 12 + 16 + 24 + 32 + 40 + 48 + 60);
#elif (0)
// 1 ring of 60, 1 ring of 24, 1 assembly of 6 rings
using MyPixelDriver = NeopixelDriver<PixelType::GRB_SEQ3>;
static const size_t nrNeopixels = (60 + 24 + 1 + 8 + 12 + 16 + 24 + 32);
#elif (1)
// 1 ring of 24
using MyPixelDriver = NeopixelDriver<PixelType::GRB_SEQ3>;
static const size_t nrNeopixels = 24;
#else
#error "No valid Neopixel configuration selected"
#endif

//------------------------------------------------
// Declare this in main.cpp,
// use as global variable in animations
//------------------------------------------------
extern MyPixelDriver npx;
