/*
 * MIT License
 *
 * Copyright (c) 2021 Mikko Johannes Heinänen
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

#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H

#if defined(__AVR_ATmega328P__)
#define _SUBEFFECTS_AVR_ATMEGA328_
#else
#error This library currently only supports AVR ATmega328 chip;      // comment out if you would still like to continue + remember to define  _SUBEFFECTS_AVR_ATMEGA328P_
#endif

/* includes for the SubEffects class */
#include <Arduino.h>
#include <inttypes.h>
#include <FastLED.h>
#include "config.h"
#include "utils/debug.h"
#include "utils/colorMath.h"
#include "utils/ledStrip.h"
#include "utils/interrupt.h"
/* -------------------------------- */

#include "Audio-modes/colorBass.h"

class SubEffects
{
private:
    bool _initialized;
    ledStrip *_strips[MAX_STRIPS];
    uint8_t _stripsPos = 0;                                 // Keeps count on the number of ledStrips allocated
    
    //bool SetModeToStrip(uint8_t modeIndex, uint8_t identifier);
    bool UpdateStripValues(uint8_t identifier);             // Updates strip's settings with the current global settings. 
    void UpdateAllStripsValues();                           // Updates all strip's settings
    uint8_t GetStripPos(uint8_t identifier);                // Returns the position of the strip in the _strips array
    ledStrip * GetStripPtr(uint8_t identifier);             // Returns the corresponding strip's pointer. If not found, returns nullpointer.
    //bool SetPalettePtr(uint8_t index,uint8_t identifier); // Returns the corresponding color palette

protected:
    uint8_t _brightValue;
    uint8_t _currentMode;
    uint8_t _currentPalette;

public:
    SubEffects(); 
    
    /* Led strip controls */
    CRGB *GetLedsPtr(uint8_t identifier);                                                        // returns CRGB* leds[] array if identifier matches to led strip
    uint8_t AddLedStrip(uint8_t audioPin, uint8_t ledMode, uint16_t ledArrSize);                 // Adds new ledStrip to the library & allocates the led array from the heap.
    uint8_t AddLedStrip(uint8_t audioPin, uint8_t ledMode, uint16_t ledArrsize, CRGB *ledArray); //
    void RemoveAllStrips();                                                                      // Removes all led strips
    bool RemoveLedStrip(CRGB *ledArray);                                                         // Removes led strip wich ledArray ptr corresponds
    bool RemoveLedStrip(uint8_t identifier);                                                     // Removes led strip with corresponding identifier
    /* ------------------ */

    void CalibrateNoise(); // Corrects for noise in signal. Calibrates all led Strips. Calibration takes 1.5 s

    /* Brightness controls */
    void SetBrightness(uint8_t brightness);
    bool IncreaseBrightness();
    bool DecreaseBrightness();

    void Update();

    /* Mode controls */
    void NextMode();
    void PreviousMode();
    void SetMode(uint8_t mode);
    bool loadMode(audioMode *ptr, uint8_t identifier);

    /* Color palette controls */
    void NextColor();
    void PreviusColor();
    bool SetColor(uint8_t paletteIndex);

    ~SubEffects();
};
#endif
