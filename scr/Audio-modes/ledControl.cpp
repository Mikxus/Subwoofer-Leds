

#ifndef LEDCONTROL_H
#include <ledControl.h>
#endif

void ledControl::Init(uint16_t led_count,int16_t updateFps)
{
    led_count = led_count;
   _updateRate = 1000000 / updateFps;
}
void ledControl::Fade(uint8_t bassHz)   
{
    if (bassHz != 0)
    {  
        uint16_t val;
        val = map(bassHz,0,255,0,359);              // implement color palettes
        _colorIncrement = val - _currentColorValue;
        if (_colorIncrement > 1)
        {

        } else if (_colorIncrement < -1)
        {

        }
    }
}

ledControl::~ledControl()
{
}