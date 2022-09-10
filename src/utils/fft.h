/*
 * MIT License
 * 
 * Copyright (c) 2021 Mikko Johannes Heinänen 
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#ifndef FFT_H
#define FFT_H

#include <Arduino.h>

#ifndef MEMORY_FREE_H
#error ("You can get the memory free library at: https://github.com/McNeight/MemoryFree")
#endif

#include <arduinoFFT.h>

#if defined( _SUBEFFECTS_AVR_ATMEGA328P_ )
    #include "avr/atmega328p/timer1.h"
#else
    #error " FFT.h only supports only avr ATmega328P";
#endif


/* Variables for the timer isr */
volatile uint16_t _fftBinSize = 64; // default size
volatile bool _fftBinReady;
double * _vReal;

class fft : public timer1
{
private:
    arduinoFFT FFT = arduinoFFT();              // Create object of arduinoFFT();
    bool _arrAllocated = 0;                     // Flag to knwo if fft bins are allocated
    double * _vImag = nullptr;                  // Pointer for the fft bin's imaginary values
    bool allocMem();
    void deallocMem();
    
protected:
    uint32_t _frequency = 700;                  // Frequency for the fft

public:
    bool SetSampleSize(uint16_t size = 64);     // Changes the fft bin size
    void SetFftFrequency(uint32_t freq = 700);     // Changes the fft frequency
    void Stop();                                // Stops the fft
    bool Init();                                // Initializes uno's timer1 for interrupts and allocates fft bins
    uint16_t Calculate();                       // Returns the loudest frequency if FFT bins are ready. Othervise it just returns 0.
    uint16_t CalculateOptimized();
    void Benchmark();
    ~fft();
};

#include "fft.cpp"

#endif
