
#ifndef LEDCONTROL_H
#define LEDCONTROL_H

class ledControl
{
private:
    uint16_t _attack = 20,_decay = 40, _sustain = 0;
    uint16_t _colorFade = 70; // fade time for the hsv color. example 0 -> 359 would take 70 ms
    uint16_t lastUpdate;
    int16_t _currentColorValue;

    uint8_t _colorIncrement;
    uint16_t _updateRate = 100;          // FPS for the fade function
    uint16_t _realUpdateRate;            // Variable for the fade functions

    uint16_t led_count;
public:
    void Fade(uint8_t bassHz = 0 );
    void SetAttack(uint16_t attack = 0);
    void SetDecay(uint16_t decay = 0);
    void Init(uint16_t led_count, int16_t updateFps); // initialize led controller   
    ~ledControl();
};

#include "ledControl.cpp"

#endif