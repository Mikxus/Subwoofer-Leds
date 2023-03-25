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
#ifndef _SUBEFFECTS_LEDSTRIP_H_
#define _SUBEFFECTS_LEDSTRIP_H_

#include <FastLED.h>
#include <inttypes.h>
#include "../config.h"
#include "../colorPalettes.h"
#include "../Audio-modes/audioModes.h"
#include "debug.h"

struct ledStrip // Struct for new led strips
{
    ledStrip() = default;

    CRGB *ledArr = nullptr; // Pointer for the led array
    uint16_t arrSize = 0;   // Number of leds on a single strip

    uint8_t inputPin = 0;      // input pin used for the led srip
    uint16_t inputCalZero = 0; // Min value above the noise level
    uint16_t inputCalMax = 0;  // Max input value
    uint8_t identifier = 0;    // value to associate strip later

    uint8_t mode = 0;         // contains light mode index
    uint8_t brightness = 255; // brightness scale     255 is equal to 100% of the led's brightness
    uint8_t paletteIndex = 0; // current palette id

    audioMode *modeUpdatePtr = nullptr; // Pointer to audio mode class
    CRGBPalette16 colorPalette = CRGBPalette16(CRGB::Black);

    /* Methods for controlling the strip */
    void SetBrightness(uint8_t value);
    bool SetColor(uint8_t index);
    bool SetMode(uint8_t modeIndex, CFastLED *fptr);

    bool loadObj(audioMode *ptr, CFastLED *fptr);

    ~ledStrip();

protected:
    bool dynAllocated = 0; // true if CRGB *ledArr is allocated by the ledStrip struct

    bool SetColorPalette(uint8_t colorIndex);
    bool loadMode(uint8_t modeIndex, CFastLED *fptr); // "assings" mode to given audioMode object
    void unloadMode();
};
#include "../Audio-modes/colorBass.h"
#endif