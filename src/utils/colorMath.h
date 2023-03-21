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

#include <Arduino.h>
#include <inttypes.h>
#include <math.h>
#include "../config.h"
#include "debug.h"

extern void fill_solid(struct CHSV* leds, int numToFill, const struct CRGB& hsvColor);


struct moving_average_filter
{
    float _window[2];
    float _sampleTotal;
    uint8_t _curReadIndex;
    float _sampleAvg;

    float calc(float val);
}; 

struct weighted_moving_average_filter
{
    float _Alpha = 0.05;    // smoothing factor
    float _result = 0.0;

    float calc(float analogRead);
};

// Expotentially weighted mowing average
struct EWMA
{
    float _alpha =  0.0F;
    float _last_val = 0.0F;    
    float _timeElapsed = 0.0F;

    EWMA(float Alpha = 0.30) : _alpha( Alpha ) {}

    float calc( float sensor_value )
    {
        _timeElapsed = (micros() - _timeElapsed) / 1000;

        float real_alpha = _alpha / ( _timeElapsed  + 1.0F );

        _last_val = (1.0F  - real_alpha) * _last_val + real_alpha * sensor_value;
        log_time();
        return _last_val;
    }
    
    void log_time() 
    {
        _timeElapsed = micros();
    }
};

struct EWMAtest
{
    float m_window_size =  0.0F;
    float m_last_val = 0.0F;
    float m_time_elapsed = 0.0F;

    /**
     * @brief Construct a new EWMAtest object
     * 
     * @param window_size rise time from 0% - ~80% signal 
     */
    EWMAtest(float window_size = 0.30) : m_window_size(window_size) {}

    /**
     * @brief Applies filtering to input value. 
     * 
     * @param input_value 
     * @return float 
     */
    float calc( float input_value )
    {
        /* Lets get the timedelta from last call */
        m_time_elapsed = (micros() - m_time_elapsed) / 1000000.0F;

        /* calculate alpha */
        float alpha = exp(m_time_elapsed / m_window_size) - 1.0F;

        if (alpha > 1.0F)
        {
            WARN(F("EWMA filter is running too slow for its window_size"));
            alpha = 1.0F;
        }

        m_last_val = alpha * input_value + (1.0F - alpha) * m_last_val;

        m_time_elapsed = micros();
        return m_last_val;
    }
};

/**
 * @brief template for a constant rate of change
 *  Initialized with max rate of change +, then the time in ms
 *  example: changeLimiter<int> limiter( 300, 20 );
 * 
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
    ~constantChangeRater() = default;
    T limit( T value );

};

template< class T> constantChangeRater<T>::constantChangeRater( T changeRate, T changeTime)
: unit( changeRate )
, changeTime( changeTime )
{
    stepSize = changeRate / changeTime;
}

template< class T> T constantChangeRater<T>::limit( T value )
{
    // Value wich tells how many steps of changetime we're going
    T timeSteps = ( millis() - lastTime ) / changeTime;

    if ( timeSteps > 1.0 ) timeSteps = 2;

    // lets calculate how many changeRates we want to go
    T changes = (value - lastValue) / unit;

    if (timeSteps < 1.0 )
    {     
        changes = constrain( changes, -1, 1);
    } else {
        if (changes < 0) changes = -1;
        else changes = 1;
        
        changes *= timeSteps;
    }


    lastValue += unit * changes * timeSteps;
    
    lastTime = millis();
    
    return lastValue;
}

#endif
