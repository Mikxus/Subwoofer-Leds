#ifndef __INC_FASTSPI_LED2_H
#include <FastLED.h>
#endif

void ledControl::ledControl(uint16_t led_count,uint16_t updateFps)
: leds(led_count) 
{
    updateRate = 1000000 / updateRate;
}
void ledControl::Fade(uint8_t bassHz)   
{
    if (micros() > lastUpdate + updateRate)
    {

        uint16_t val;
        val = map(bassHz,0,255,0,359);
        _colorIncrement = abs(_currentValue - val); // kesken

    }
}

ledControl::~ledControl()
{
}