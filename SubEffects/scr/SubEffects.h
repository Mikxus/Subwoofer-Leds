#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H

#include <Arduino.h>

#define DEBUG                                                 // Uncomment to enable DEBUG serial prints

class timer1                                                     // Manages the arduino uno's timer1
{
    protected:

    public:

    void Start(uint16_t _size, uint16_t freq);
    void Stop();

    ~timer1();                                                  // Deconstructor
};

#include "/home/mikko/Documents/code/Subwoofer-Leds/SubEffects/scr/Audio-modes/fft/fft.h"   // ! has to be declared here

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
class SubEffects : public Modes
{
protected:
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    SubEffects(uint8_t subPin = 0,uint8_t led_dataPin = 8,uint16_t led_count = 0);
    ~SubEffects();
};

#include "SubEffects.cpp"
#endif 
