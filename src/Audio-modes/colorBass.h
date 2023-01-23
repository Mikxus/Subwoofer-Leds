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
#include <inttypes.h>
#include "audioModes.h"
//#include "../utils/approxFFT.h"
#include "../utils/colorMath.h"
#include "../utils/ledStrip.h"
#include "../utils/debug.h"
#include "../utils/FFT.h"
/* -------- */


class colorBass : public audioMode                     // Simple bass effect
{
    uint16_t _lastBrightness = 0;
    bool _update = 0;
    uint16_t _lastFreq = 0;

    FFT fft_obj;
    
private:
    inline uint8_t fade(uint16_t freq, uint16_t brightness);
    inline void logLastValue(uint8_t hue, uint8_t saturation, uint8_t value);

    EWMA bright = EWMA(0.20);
    EWMA bright2 = EWMA(0.27);
    //constantChangeRater<float> color_smooth = constantChangeRater<float>( 1024.0F,  170.0F);
    EWMA color_smooth = EWMA(0.01);
    /* Last Values */
    uint8_t m_last_r;
    uint8_t m_last_g;
    uint8_t m_last_b;

public:
    colorBass();
    virtual bool Update();                             // Calculates values for the led

    ~colorBass() = default;    
};

#endif