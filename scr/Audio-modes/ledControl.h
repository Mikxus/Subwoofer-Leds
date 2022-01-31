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

#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#ifndef __INC_FASTSPI_LED2_H
#include <FastLED.h>
#endif

uint16_t _deltaTime;                       // Variable for the fade functions
/*
enum smoothBrightness
{
    10 = 1.5,
    9 = 1.0,
    8 = 0.5,
    7 = 0.35,
    6 = 0.1;
    5 = 0.035,
    4 = 0.015;
    3 = 0.01,
    2 = 0.005,
    1= 0.0001
};
*/
struct moving_average_filter
{
    float _window[2];
    float _sampleTotal;
    uint8_t _curReadIndex;
    float _sampleAvg;
    float calc(float val);
};

float moving_average_filter::calc(float val)
{
    _sampleTotal = _sampleTotal - _window[_curReadIndex];   // subtract the last reading
    _window[_curReadIndex] = val;                           // add the latest value
    _sampleTotal += _window[_curReadIndex];                  // add the reading to the total
    _sampleAvg = _sampleTotal / 2.0;                          // calculate the average
    _curReadIndex += 1;
    if ( _curReadIndex >= 2 ) _curReadIndex = 0;
    return _sampleAvg;
}   

struct weighted_moving_average_filter
{
    float _Alpha = 0.1;
    float _result = 0.0;
    float _r = 0.04;
    float calc(float analogRead);
};

float weighted_moving_average_filter::calc(float val)
{
    /*_r = -( _deltaTime / log( 1 - _Alpha ) );      // calculate the smoothing factor
    _Alpha = _deltaTime / _r;*/
    _Alpha = _r * ( ( float ) _deltaTime / 1000000.0 + 1.0);
    _result = _Alpha * val + ( 1.0 - _Alpha ) * _result;
    return _result;
}


class ledControl
{
private:
    void logTime();                                 // saves the time. Used for determining the time delta
    void logLastHSV(uint8_t hue = 0, uint8_t saturation = 255, uint8_t value = 0);
    
    uint8_t _lastHue;
    uint8_t _lastSaturation;
    uint8_t _lastValue;
    bool _breathDir;           // 0 for down 1 for up
                                  //  saves the last HSV values
protected:
    weighted_moving_average_filter bright, color;

    float _lastUpdate;

    int16_t _currentColorValue;
    int16_t _colorChange;                           // holds value of change
    uint16_t _updateRate = 100;                     // refresh rate for the fade function
    uint16_t _led_count;
    CFastLED* _FastLED;  
    CRGB* _leds;                                    // Pointer which will hold the FastLED object.

public:
    void Fade(uint8_t bassHz = 0,uint16_t brightness = 0);  // Fading function which smoothens the input and then updates all leds to the same value.
    void SetAttack(uint16_t attack = 0);
    void SetDecay(uint16_t decay = 0);
    void Init(uint8_t led_dataPin ,uint16_t led_count, CFastLED & fastLedObj, CRGB ledObj[]); // initialize led controller   


    // Led animations

    void BreathingStatic(uint8_t hue = 0, uint8_t saturation = 255,uint8_t fromLow = 0, uint8_t toHigh = 255);
    


    ~ledControl();
};

#include "ledControl.cpp"

#endif