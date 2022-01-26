#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H


#include <Arduino.h>
//#define DEBUG                             // Uncomment to enable DEBUG serial prints

#ifndef LEDCONTROL_H
#include "Audio-modes/fft.h"                // ! has to be declared here
#include "Audio-modes/ledControl.h"
#endif

long int _calibratedNoiseZero;

uint8_t _subwooferPin;      // global variable for subwoofer pin
                            // used in timer1's ISR

class Modes : public fft
{
    private:
    bool _initialized;

    protected:
    uint8_t _currentMode;
    uint8_t _modeCount;

    public:
    ledControl ledController;
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
    //uint8_t _ledDataPin;
    //uint16_t _ledCount;

public:

    SubEffects(uint8_t subPin, uint8_t led_dataPin,uint16_t led_count, CFastLED & fastLedObj, CRGB ledObj[]);
    ~SubEffects();
    void CalibrateSoundLevel();      // Corrects for dc offset in signal. When called it records for 1.5 the highest voltage.
};
#include "SubEffects.cpp"

#endif 
