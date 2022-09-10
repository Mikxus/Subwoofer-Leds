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


#include "colorBass.h"


colorBass::colorBass()
{
    if ( FFT.Init() )
    {
        #ifdef DEBUG
        Serial.println(F("FFT initialized succesfully"));
        #endif
        _initFlag = 1;
    } else 
    {
        #ifdef DEBUG
        Serial.println(F("FFT failed to be initialized"));
        digitalWrite(13,HIGH);
        #endif
    }
    _calibratedNoiseZero = _ledStrip->inputCalZero;
    ledController.Init(_FastLED,_ledStrip);
}

/**
 * @brief Changes leds color based on the frequency of bass. Led brightness is controlled by the magnitude
 *        of the input signal.
 *
 *
 * @returns booL: 1 If value has changed. 
 */
bool colorBass::Update()
{
    uint16_t freq = 0;

    if (_update == 1)
    {
        freq = FFT.Calculate();
    }
    
    if (freq == 0) freq = _lastFreq;
    else _lastFreq = freq;

    uint16_t brightness = analogRead(_ledStrip->inputPin);

    if (brightness <= _calibratedNoiseZero) freq = _lastFreq;

    brightness = constrain(brightness, _ledStrip->inputCalZero, 800);
    brightness = map(brightness, _ledStrip->inputCalZero,550,0,255);
    

    if (brightness == _lastBrightness)
    {
        if (! ledController.Fade(freq, (uint8_t) brightness) ) 
        {
            _update = 0;
            return 0;
        } else 
        {
            _update = 1;
            return 1;
        }
    }

    if (_update == 0) _update = 1;

    _lastBrightness = brightness;

    /* Applies smoothing to color & brightness. 
    Then applies resulting color to ledArr */
    return ledController.Fade(freq, brightness);
}
