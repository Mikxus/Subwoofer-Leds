

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
        uint16_t val,led_brightness;
        val = map(bassHz,0,255,0,359);              // implement color palettes
        _colorIncrement = val - _currentColorValue;
        led_brightness = analogRead(_subwooferPin);

        if (_colorIncrement > 0)
        {

        } else if (_colorIncrement < 0)
        {
            for (uint16_t i = 0; i < led_count; i++ )
            {
                leds[i] = 
            } 
        } else          // When equal to zero
        {

        }
        _lastUpdate = millis();
    }
}

ledControl::~ledControl()
{
}