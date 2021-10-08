#ifndef T51223
#define T51223

#include <Arduino.h>

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints



class Modes
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

class timer1                                                     // Manages the arduino uno's timer1
{
    protected:
        uint16_t _frequency;

    public:

    void Start();
    void Stop();
    ~timer1();                                                  // Deconstructor
};

class fft
{
    private:
    uint16_t _arrPos;
    uint16_t _sampleSize;
    bool _arrReady;
    bool _arrAllocated;                                           // keep track if memory is allocated for fft'
    double *_vReal,*vImag;

    void ISR(TIMER1_COMPB_vect);

    public:
    
    void SetSampleSize(uint16_t size = 64);
    void SetFrequency(uint16_t freq = 700);
    void Stop();
    bool Init();
    void Calculate();
    ~fft();
};

// Base class
class SubEffects : public timer1, public Modes
{
protected:
    uint16_t _sampleSize;
    uint8_t _subwooferPin;
    uint8_t _ledDataPin;
    uint16_t _ledCount;
public:
    SubEffects(uint8_t subPin = 0,uint8_t led_dataPin = 8,uint16_t led_count = 0);
    ~SubEffects();
};

#endif 
