#include <Arduino.h>

#ifndef FFT_H
#define FFT_H

//#include "timer1/timer1.h"
#include <MemoryFree.h>

#ifndef MEMORY_FREE_H
#pragma ("You can get the memory free library at: https://github.com/McNeight/MemoryFree")
#endif

#include "timer1.h"
#include <arduinoFFT.h>

//#include "/home/mikko/Documents/code/Subwoofer-Leds/SubEffects/scr/SubEffects.h"
volatile uint16_t _fftBinSize = 64; // default size
volatile bool _fftBinReady;
double * _vReal;

class fft : public timer1
{
    private:
    arduinoFFT FFT = arduinoFFT();
    bool _arrAllocated;                     // value which track if memory is allocated for fft's bins
    double * _vImag;                        // pointer for the fft bin's imaginary values
    bool allocMem();
    void deallocMem();
    protected:
    uint16_t _frequency = 700; // default value

    public:
    bool SetSampleSize(uint16_t size = 64);     // Changes the fft bin size
    void SetFrequency(uint16_t freq = 700);     // Changes the fft frequency
    void Stop();                                // Stops the fft
    bool Init();                                // Initializes timer1 for interrupt and allocates fft bins
    uint16_t Calculate();
    void Benchmark();
    ~fft();
};

#include "fft.cpp"

#endif