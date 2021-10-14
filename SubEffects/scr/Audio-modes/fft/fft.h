#include <Arduino.h>

#ifndef FFT_H
#define FFT_H

#include "D:\Code\Github\Subwoofer-Leds\SubEffects\scr\SubEffects.h"

class timer1;

class fft : public timer1
{
    private:
    bool _arrAllocated;                                           // keep track if memory is allocated for fft'
    double * _vImag;
    double * _vReal;

    bool allocMem();
    void deallocMem();
    protected:
    public:
    
    uint16_t _frequency = 700; // default value
    uint16_t _sampleSize = 64; // default value
    bool _arrReady;
    uint16_t _arrPos;

    bool SetSampleSize(uint16_t size = 64);
    void SetFrequency(uint16_t freq = 700);
    void Stop();
    bool Init();
    void Calculate();
};

#include "fft.cpp"

#endif