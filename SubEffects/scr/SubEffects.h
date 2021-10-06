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


    bool SubEffects(uint8_t pin = 0,samples = 64,freq = 700);
    ~SubEffects();
};

class Modes
{
    private:
    bool _initialized;

    protected:
    uint8_t _currentMode;
    uint8_t _modeCount;

    public:
    
    void update();              

    void NextMode();
    void PreviousMode();
    void SetMode(uint8_t mode);

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

class fft : public SubEffects
{
    private:
    bool arrAllocated;                                           // keep track if memory is allocated for fft'

    public:
    void Stop();
    bool Init();
    void Calculate();
    ~fft();
}
#endif 
