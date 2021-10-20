#include <Arduino.h>

#ifndef FFT_H
#define FFT_H

//#include "timer1/timer1.h"
#include <MemoryFree.h>
#include "timer1.h"
#include <arduinoFFT.h>


//#include "/home/mikko/Documents/code/Subwoofer-Leds/SubEffects/scr/SubEffects.h"
volatile uint16_t _fftBinSize = 64; // default size
volatile bool _fftBinReady;
volatile double * _vReal;

class fft : public timer1
{
    private:
    arduinoFFT FFT = arduinoFFT();
    bool _arrAllocated;                                           // keep track if memory is allocated for fft'
    double * _vImag;
    bool allocMem();
    void deallocMem();
    protected:
    uint16_t _frequency = 700; // default value

    public:
    bool SetSampleSize(uint16_t size = 64);     // Changes the fft bin size
    void SetFrequency(uint16_t freq = 700);     // Changes the fft frequency
    void Stop();                                // Stops the fft
    bool Init();                                // Initializes timer1 for interrupt and allocates fft bins
    void Calculate();
    void Benchmark();
    ~fft();
};

#include "fft.cpp"

#endif