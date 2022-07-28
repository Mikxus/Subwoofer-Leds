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
    ledController.Init(_FastLED,_ledStrip);            // initialize ledcontroller
}

bool colorBass::Update()
{
    uint16_t freq = 0;

    if (_update == 1)
    {
        freq = FFT.Calculate();                                         // Get the frequency
    }
    
    if (freq == 0) freq = _lastFreq;                                    // if the frequency value is 0 use the last value;
    else _lastFreq = freq;

    uint16_t brightness = analogRead(_ledStrip->inputPin);              // Get the amplitude of signal

    if (brightness <= _calibratedNoiseZero) freq = _lastFreq;                   // if the amplitude is below the calibrated noise zero value, set the frequency to last one 

    brightness = constrain(brightness, _ledStrip->inputCalZero, 800);   // then constrain it to predefined range
    brightness = map(brightness, _ledStrip->inputCalZero,550,0,255);    // then map it
    

    if (brightness == _lastBrightness)                                  // The fade method will not stay 0 always, it will fluctuati little bit
    {                                                                   // so we check if the fade hits "bottom" then not allowing it to fluctuate by not calling it
        //Serial.println(F("Calling Fade"));
        //Serial.flush();
        if (! ledController.Fade(freq, (uint8_t) brightness) ) 
        {
           // Serial.println(F("Exited Fade = 0"));
           // Serial.flush();
            _update = 0;
            return 0;          // until brightness changes
        } else 
        {
            //Serial.println(F("Exited Fade = 1"));
           // Serial.flush();
            _update = 1;
            return 1;
        }
    }

    if (_update == 0) _update = 1;

    _lastBrightness = brightness;

    //Serial.println(F("Calling Fade2"));
    //Serial.flush();
    if ( ledController.Fade(freq, brightness) ) {

        return 1;             // Calls the Fade function in ledController. Returns 1 if any value was updated
    } else
    {
        return 0;
    }
}
