
#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <FastLED.h>

class ledControl
{
private:
uint16_t _attack = 10,_decay = 10;
extern CRGB leds[];


public:
    void Fade(uint16_t color = 0 );
    void SetAttack(uint16_t attack = 0);
    void SetDecay(uint16_t decay = 0);
    ledControl();
    ~ledControl();
};

#include "ledControl.cpp"
#endif