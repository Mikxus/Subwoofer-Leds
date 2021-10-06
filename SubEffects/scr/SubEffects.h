#ifndef SubEffects
#define SubEffects

#include <Arduino.h>
#include <SubEffects.h>

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints


class SubEffects : public timer1, public mode
{
protected:
    uint8_t _data_pin;
    uint16_t _led_amount;
    uint8_t _subwoofer_pin;

    CRGB leds[];

public:
    
    bool SubEffects(uint8_t subwoofer_in = 0,uint8_t led_pin,uint16_t led_amount);
    bool update();                                               // Runs the calculations then updates the leds
    ~SubEffects();
};

class modes
{
    protected:
    uint8_t _currentMode;
    public:
    void update();
    void ChangeMode(uint8_t mode);                                // change mode to given value
    void NextMode();                                              // Next mode
    void PreviousMode();                                           // Previous mode
    void modes();
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
    double *_vReal,vImag;

    public:

    bool init();
    void calculate();
    ~fft();
}
#endif 
