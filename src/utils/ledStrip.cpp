/*
 * MIT License
 *
 * Copyright (c) 2022 Mikko Johannes Heinänen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ledStrip.h"

/**
 * @brief Sets brightness
 *
 * @param value
 */
void ledStrip::SetBrightness(uint8_t value)
{
    brightness = value;
    return;
}

bool ledStrip::SetColor(uint8_t index)
{
    if (index >= NUM_OF_PALETTES)
    {
        ERROR(F("ledstrip.cpp: Colorpalette index not found\n\rInput: "), index, F("\n\rNumber of palettes: "), NUM_OF_PALETTES);
        return 0;
    }
    paletteIndex = index;
    if (!SetColorPalette(paletteIndex))
    {
        ERROR(F("ledstrip.cpp: Failed to set color palette\n\rIndex: "), paletteIndex, F("\n\rStrip id: "), identifier);
    }

    INFO(F("Colorpalette: "), paletteIndex, F(" set for strip: "), identifier, F("\n\r"));
    return 1;
}

bool ledStrip::SetMode(uint8_t modeIndex, CFastLED *fptr)
{
    if (modeIndex >= NUM_OF_MODES)
        return 0; // if invalid input return 0

    if (!loadMode(mode, fptr)) // check if fails
    {
        ERROR("ledStrip.cpp: Failed to set mode: ", modeIndex);
        return 0;
    }
    return 1;
}

bool ledStrip::loadMode(uint8_t modeIndex, CFastLED *fptr)
{
    // Check if CFastLED pointer is valid
    if (fptr == nullptr)
        return 0;

    // check if modeUpdatePtr is already loaded

    if (modeUpdatePtr != nullptr)
    {
        // unload mode
        INFO(F("Mode already loaded for strip: "), identifier, F(" unloading current mode:"), mode);
        unloadMode();
    }

    switch (modeIndex) // switch for the different modes
    {
    case 0: // colorBass mode

        modeUpdatePtr = new colorBass();

        break;

    default:
        return 0;
        break;
    }

    if (modeUpdatePtr == nullptr)
    {
        ERROR(F("ledStrip: Not enoug memory for mode: "), modeIndex);
        return 0;
    }

    // Share ledstrip & FastLED pointer with the new mode.
    modeUpdatePtr->initValues(this, fptr);

    mode = modeIndex;
    INFO(F("Succesfully loaded mode: "), modeIndex, F(" for strip: "), identifier);
    return 1;
}

/**
 * @brief Loads allocated mode to ledStrip.
 *
 * @param ptr
 * @param fptr
 * @return true
 * @return false
 */
bool ledStrip::loadObj(audioMode *ptr, CFastLED *fptr)
{
    // check if modeUpdatePtr is already loaded
    if (modeUpdatePtr != nullptr)
    {
        // unload mode
        INFO(F("Mode already loaded for strip: "), identifier, F(" unloading current mode:"), mode);
        unloadMode();
    }

    modeUpdatePtr = ptr;
    if (modeUpdatePtr == nullptr)
    {
        ERROR(F("ledStrip: null audio mode"));
        return 0;
    }
    modeUpdatePtr->initValues(this, fptr);
    INFO(F("Succesfully loaded mode: "), reinterpret_cast<uint16_t>(ptr), F(" for strip: "), identifier);
    return 1;
}

void ledStrip::unloadMode()
{
    if (modeUpdatePtr == nullptr)
        return;

    delete modeUpdatePtr;
    modeUpdatePtr = nullptr;
    return;
}

bool ledStrip::SetColorPalette(uint8_t colorIndex)
{
    switch (colorIndex) // add new case for your color palette. !color index can't jump any digits
    {
    case 0:
        colorPalette = blueBass_p;
        return 1;

    default:
        ERROR(F("ledStrip: color palette not found. index: "), colorIndex);
        return 0;
        break;
    }
    return 0;
}

ledStrip::~ledStrip()
{
    unloadMode();
    if (dynAllocated)
        free(ledArr);
    return;
}
