#ifndef SubEffects
#define SubEffects

#include <Arduino.h>
#include <SubEffects.h>

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints


class SubEffects : public timer1, public mode
{
protected:
    uint16_t _sampleSize;
    uint8_t _subwooferPin;
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    bool SubEffects(uint8_t pin = 0,uint8_t led_dataPin,uint16_t led_count);
    ~SubEffects();
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
    ~timer1();                                                  // Deconstructor
}

class fft : public SubEffects, timer1
{
    private:
    uint16_t _sampleSize;
    bool _arrAllocated;                                           // keep track if memory is allocated for fft'
    double *_vReal,*vImag;

    public:
    void SetSampleSize(uint16_t size = 64);
    void SetFrequency(uint16_t freq = 700);
    void Stop();
    bool Init();
    void Calculate();
    ~fft();
}
#endif 
