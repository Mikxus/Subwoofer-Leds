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


#ifndef _COLOR_MATH_H_
#define _COLOR_MATH_H_

#include <FastLED.h>
#include "ledStrip.h"
#include <math.h>
#include <sys/types.h>


extern void fill_solid(struct CHSV* leds, int numToFill, const struct CRGB& hsvColor);


float _lastTime;                       // Variable for the fade functions
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

    // declare _alpha and _fading as floats
    float _Alpha = 0.05;    // smoothing factor
    //float _fading = 0.0;    // fading factor


    //float _Alpha = 0.4;
    float _result = 0.0;
    //float _fading = 1.0;
    float calc(float analogRead);
};

// Create function that takes in a float then smooths it, with given smoothing factor
// comment the output range of the function
float weighted_moving_average_filter::calc(float analogRead)
{
    _result = _Alpha * analogRead + (1 - _Alpha) * _result;
    return _result;
    // output range: 0.0 to 1.0
}

// Expotentially weighted mowing average

struct EWMA
{
    float _alpha = 0.26;        // Lower value smoother
    float _lastValue = 0;    
    float _deltaTime = 0;

    EWMA(float Alpha = 0.26) : _alpha( Alpha ) {}
    float calc( float sensorValue ) {
        _deltaTime = micros() - _deltaTime;
        
        _deltaTime /= 10000.0;

        if (_deltaTime > 1.0) _deltaTime = 1.0;

        float real_alpha =  _deltaTime / exp(1.0 - _alpha);


        _lastValue = (1.0F - real_alpha) * _lastValue + real_alpha * sensorValue;
        log_time();
        return _lastValue;
    }

    void log_time() {
        _deltaTime = micros();
    }
};


/* template for a constant rate of change
    Initialized with max rate of change +, then the time in ms
    example: changeLimiter<int> limiter( 300, 20 );

*/

template< class T > class constantChangeRater {
private:
    T unit; 
    T changeTime;       // lenght of changeRate going
    T stepSize;         // Stepsize per ms
    uint32_t lastTime = 0;

    T lastValue = 0;
public:
    constantChangeRater( T changeRate, T changeTime );       // max change in ms
    ~constantChangeRater();
    T limit( T value );
};


class ledControl                        
{
private:
    void logTime();                                 // saves the time. Used for determining the time delta
    inline void logLastValue(uint8_t hue = 0, uint8_t saturation = 255, uint8_t value = 0) __attribute__ ((always_inline));  // logs current hsv values
    
    uint8_t _lastHue;
    uint8_t _lastSaturation;
    uint8_t _lastValue;
    bool _breathDir;           // 0 for down 1 for up

protected:
    EWMA bright = EWMA(0.19);
    EWMA bright2 = EWMA(0.25);

    constantChangeRater<float> color = constantChangeRater<float>( 1,  15);

    float _lastUpdate = 0.0;

    ledStrip* _strip;
    CFastLED* _FastLED;  

public:
    void Init( CFastLED* fastLedObj, ledStrip* strip);
    bool Fade(uint8_t bassHz = 0,uint8_t brightness = 0);  // Fading function which smoothens the input and then updates all leds to the same value.
    void SetAttack(uint16_t attack = 0);
    void SetDecay(uint16_t decay = 0);
    void Init(uint8_t led_dataPin ,uint16_t led_count, CFastLED & fastLedObj, CRGB ledObj[]); // initialize led controller   
    

    ledControl();
    ~ledControl();
};

#include "colorMath.cpp"

#endif