#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H


#ifndef __INC_FASTSPI_LED2_H
#include <FastLED.h>
#endif



#include <Arduino.h>
//#define DEBUG                                  // Uncomment to enable DEBUG serial prints

#ifndef LEDCONTROL_H
#include "Audio-modes/fft.h"   // ! has to be declared here
#endif
#include "Audio-modes/ledControl.h"

class Modes : public fft , public ledControl
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

class SubEffects : public Modes
{
protected:
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    SubEffects(uint8_t subPin = 0,uint16_t led_count = 0,uint8_t led_dataPin = 6);
    ~SubEffects();
};
#include "SubEffects.cpp"

#endif 
