#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H

#include <Arduino.h>

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints

class Modes;
class timer1;
class SubEffects;
class fft;

class timer1                                                     // Manages the arduino uno's timer1
{
    protected:

    public:

    void Start(uint16_t _size, uint16_t freq);
    void Stop();

    ~timer1();                                                  // Deconstructor
};

class fft
{
    private:
    bool _arrAllocated;                                           // keep track if memory is allocated for fft'
    double *vImag;

    public:
    
    uint16_t _frequency;
    uint16_t _sampleSize;
    double *_vReal;
    bool _arrReady;
    uint16_t _arrPos;

    void SetSampleSize(uint16_t size = 64);
    void SetFrequency(uint16_t freq = 700);
    void Stop();
    bool Init();
    void Calculate();
};

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
    ~Modes();
};
// Base class
class SubEffects : public timer1, public Modes
{
protected:
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    uint16_t _sampleSize;
    uint8_t _subwooferPin;
    SubEffects(uint8_t subPin = 0,uint8_t led_dataPin = 8,uint16_t led_count = 0);
    ~SubEffects();
};
#include "SubEffects.cpp"
#endif 
