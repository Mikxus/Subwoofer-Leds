#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H

#include <Arduino.h>
//#define DEBUG                                  // Uncomment to enable DEBUG serial prints

#include "Audio-modes/fft.h"   // ! has to be declared here
#include "Audio-modes/ledControl.h"

class Modes : public fft
{
    private:
    bool _initialized;

    protected:
    uint8_t _currentMode;
    uint8_t _modeCount;

    public:
    
    void Update();              
    void NextMode();
    void PreviousMode();
    void SetMode(uint8_t mode);

    Modes();
    //~Modes();
};
// Base class
class SubEffects : public Modes : ledControl
{
protected:
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    SubEffects(uint8_t subPin = 0,uint8_t led_dataPin = 6,uint16_t led_count = 20);
    ~SubEffects();
};

#include "SubEffects.cpp"
#endif 
