#ifndef SubEffects
#define SubEffects
#include <Arduino.h>
#include <SubEffects.h>

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints


class SubEffects : public timer1, public mode
{
protected:
    uint16_t _sample_size;
    uint8_t _subwoofer_pin;
    double *_vReal,vImag;
public:


    bool Init(uint8_t pin = 0,samples = 64,freq = 700);
    ~SubEffects();
};

class modes
{
    protected:
    uint8_t _currentMode;
    public:

    void fft();
    void modes(uint8_t mode = 0);
    ~modes();
}

class timer1                                                     // Manages the arduino uno's timer1
{
    protected:
        uint16_t _frequency;

    public:

    void Start();
    void Stop();
    ~timer1();                                                   // Deconstructor
}






#endif 
