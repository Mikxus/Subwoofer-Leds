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

#ifndef _COLORBASS_H
#define _COLORBASS_H

/* includes */
#include "inttypes.h"
#include "../utils/approxFFT.h"
#include "../utils/colorMath.h"
#include "audioModes.h"
/* -------- */


class colorBass : public audioMode                     // Simple bass effect
{
    uint16_t _lastBrightness = 0;
    bool _update = 0;
    bool _initFlag = 0;
    uint16_t _lastFreq = 0;


    ledControl ledController;
    approxFFT FFT;                                     // fft object;
    
public:
    colorBass();
    virtual bool Update();                             // Calculates values for the led

    ~colorBass() = default;    
};
#include "colorBass.cpp"
#endif