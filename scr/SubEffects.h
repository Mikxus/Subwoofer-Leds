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


#include <Arduino.h>
//#define DEBUG                             // Uncomment to enable DEBUG serial prints

#ifndef LEDCONTROL_H
#include "Audio-modes/fft.h"                // ! has to be declared here
#include "Audio-modes/ledControl.h"
#endif

uint16_t _calibratedNoiseZero;

uint8_t _subwooferPin;      // global variable for subwoofer pin
                            // used in timer1's ISR

class Modes : public fft
{
    private:
    bool _initialized;

    protected:
    uint8_t _currentMode;
    uint8_t _modeCount;

    public:
    ledControl ledController;
    void Update();
    void NextMode();
    void PreviousMode();
    void SetMode(uint8_t mode);

    Modes();
    //~Modes();
};
// Base class

class SubEffects : public Modes
{
protected:
    //uint8_t _ledDataPin;
    //uint16_t _ledCount;

public:
    SubEffects(uint8_t subPin, uint8_t led_dataPin,uint16_t led_count, CFastLED & fastLedObj, CRGB ledObj[]);
    ~SubEffects();
    void CalibrateSoundLevel();      // Corrects for dc offset in signal. When called it records for 1.5 seconds to find the highest voltage.
};
#include "SubEffects.cpp"

#endif 
