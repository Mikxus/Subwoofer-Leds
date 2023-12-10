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
    : fft_obj(0, 64, 800, fixed_8)
{
}

/**
 * @brief Changes leds color based on the frequency of bass. Led brightness is controlled by the magnitude
 *        of the input signal.
 *
 *
 * @returns bool: 1 If value has changed.
 */
bool colorBass::update()
{
    uint16_t freq = 0;
    uint16_t brightness = analogRead(0);

    if (_update)
    {
        freq = fft_obj.calculate();
    }

    if (freq == 0) 
    {
        freq = _lastFreq;
    } else
    {
        _lastFreq = freq;
        DEBUG(F("Freq: "), freq);
    }

    // 414 is the input calzero
    brightness = constrain(brightness, 414, 800);
    brightness = map(brightness, 414, 550, 0, 255);

    /* if no change in brightness */
    if (brightness == _lastBrightness)
    {
        /* check if fade is complete */
        if (!fade(freq, (uint8_t) brightness))
        {
            _update = 0;
            return _update;
        }
        _update = 1;
        return _update;
    }

    /* Brightness changed */
    _lastBrightness = brightness;

    _update = fade(freq, brightness);
    return _update;
}

/**
 * @brief Smoothens the inputted values
 *
 * @param hue
 * @param brightness
 */
uint8_t colorBass::fade(uint16_t hue, uint16_t brightness)
{
    /* Smoothen the input values */
    float val = bright1.calc((float)brightness);
    float colorVal = color_smooth.calc((float)hue);

    /* Limit color value */
    if (colorVal > 255)
        colorVal = 255;
    if (colorVal < 0)
        colorVal = 0;

    /* Fill led_array with the smoothed values */
    fill_solid(&led_array[0], led_array.size(),
           ColorFromPalette(color_palette,
                        (uint8_t) colorVal,
                        (uint8_t) val,
                        LINEARBLEND));

    /* Check if no rgb values have changed */
    if (led_array[0].r == m_last_r && led_array[0].g == m_last_g && led_array[0].b == m_last_b)
        return 0;

    logLastValue(led_array[0].r,
                 led_array[0].g,
                 led_array[0].b);

    return 1;
}

inline void colorBass::logLastValue(uint8_t r, uint8_t g, uint8_t b)
{
    m_last_r = r;
    m_last_g = g;
    m_last_b = b;
    return;
}