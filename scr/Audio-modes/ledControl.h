
#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <FastLED.h>

class ledControl : public FastLED
{
private:
uint16_t _attack = 25000,_decay = 60000;        // Attack and decay in µs for leds brightness
uint16_t _colorFade = 70000;    // The fade time in µs from 0 to 359
CRGB leds[];

uint16_t _lastUpdate;
float _currentValue;

public:
    void FadeFFT(uint16_t bassHz = 0 );
    void SetAttack(uint16_t attack = 0);
    void SetDecay(uint16_t decay = 0);
    ledControl();
    ~ledControl();
};

#include "ledControl.cpp"
#endif