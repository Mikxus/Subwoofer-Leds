#ifndef __INC_FASTSPI_LED2_H
#include <FastLED.h>
#endif

ledControl::ledControl()
{
}
void ledControl::Fade(uint16_t color)   
{
    _lastUpdate = micros() - _lastUpdate;       // Calculate the time between updates
        uint16_t targetColor;
        float increment;
        _colorIncrement = abs(_currentValue - val); // kesk
}

ledControl::~ledControl()
{
}