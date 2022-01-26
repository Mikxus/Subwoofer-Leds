#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#ifndef __INC_FASTSPI_LED2_H
#include <FastLED.h>
#endif

uint16_t _deltaTime;                       // Variable for the fade functions

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
    float _r = 0.035;
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

    /*
    uint16_t _attack = 20,_decay = 40, _sustain = 0;
    uint16_t _colorFade = 70;                       // fade time for the hsv color. example 0 -> 359 would take 70 ms
    */
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