#ifndef SubFFT
#define SubFFT

#include "SubFFT.cpp"

//#define DEBUG                                                 // Uncomment to enable DEBUG serial prints


class SubFFT : public timer1
{
protected:
    uint16_t _sample_size;
    uint8_t _subwoofer_pin;
    double *_vReal,vImag;
public:


    bool Begin(uint8_t pin = 0,samples = 64,freq = 700);
    ~SubFFT();
};

class timer1                                                     // Manages the arduino uno's timer1
{
    protected:
        uint16_t _frequency;

    public:

    bool timer1(uin8_t frequency = 700,double *sample);          // Constructor
    ~timer1();                                                   // Deconstructor
}






#endif 