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
: fft_obj(0, 64, 700, fixed_8)
{
}

/**
 * @brief Changes leds color based on the frequency of bass. Led brightness is controlled by the magnitude
 *        of the input signal.
 *
 *
 * @returns bool: 1 If value has changed. 
 */
bool colorBass::Update()
{
    uint16_t freq = 0;

    if (_update == 1)
    {
        freq = fft_obj.calculate();
    }

    if (freq == 0) freq = _lastFreq;
    else { _lastFreq = freq; DEBUG(F("Freq: "), freq); }    

    uint16_t brightness = analogRead(_ledStrip->inputPin);

    if (brightness <= _ledStrip->inputCalZero) freq = _lastFreq;

    brightness = constrain(brightness, _ledStrip->inputCalZero, 800);
    brightness = map(brightness, _ledStrip->inputCalZero,550,0,255);
    
    /* if no change in brightness */
    if (brightness == _lastBrightness)
    {
        /* check if fade is complete */
        if (! fade(freq, (uint8_t) brightness) ) 
        {
            _update = 0;
            return _update;
        } 
        _update = 1;
        return _update;
    }

    if (_update == 0) _update = 1;

    _lastBrightness = brightness;

    /* Applies smoothing to color & brightness. 
    Then applies resulting color to ledArr */
    return fade(freq, brightness);
}


/**
 * @brief Smoothens the inputted values
 * 
 * @param hue 
 * @param brightness 
 */
uint8_t colorBass::fade(uint16_t hue, uint16_t brightness) {

    /* Smoothen the input values */
    float val = bright2.calc( bright1.calc((float)brightness) );                   // Calculate the smoothing
    float colorVal = color_smooth.calc((float)hue);
    
    /* Limit color value */
    if ( colorVal > 255 ) colorVal = 255;
    if ( colorVal < 0 ) colorVal = 0;

    /* Fill ledArr with the smoothed values */
    fill_solid( _ledStrip->ledArr, _ledStrip->arrSize, ColorFromPalette( _ledStrip->colorPalette, (uint8_t) colorVal,(uint8_t) val, LINEARBLEND ));   // Sets whole strip to same color using a color palette

    /* Check if any rgb values have changed */
    if ( _ledStrip->ledArr->r == m_last_r && _ledStrip->ledArr->g == m_last_g && _ledStrip->ledArr->b == m_last_b ) return 0;   // if no value changed return 0

    logLastValue( _ledStrip->ledArr->r , _ledStrip->ledArr->g, _ledStrip->ledArr->r );                       // save the last hsv values

    return 1;
}

inline void colorBass::logLastValue(uint8_t r, uint8_t g, uint8_t b)
{
    m_last_r = r;
    m_last_g = g;
    m_last_b = b;
    return;
}