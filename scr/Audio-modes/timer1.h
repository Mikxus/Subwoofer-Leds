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

#ifndef TIMER1_H
#define TIMER1_H

#ifndef __AVR_ATmega328P__                       // check for processor type
#warning "Timer1 library currently only supports ATmega328p based boards";
#endif

class timer1                                    // Manages the arduino uno's timer1
{
    private:
    uint32_t _achievedFrequency;                // Stores the achieved frequency
    const uint16_t prescalers[5] = { 1, 8, 64, 256, 1024};
    void SetPrescaler(uint16_t value);          // Sets the prescaler
    
    public:
    uint32_t GetTimerFrequency();               // Returns the frequency achieved | atm doesn't work
    void SetTimerFrequency(uint32_t frequency);                   // Sets the given frequency
    void Start(uint32_t freq);                  // initializes the timer1's settings | Returns the hz it was able to set
    void Stop();                                // turns off the timer
    void Continue();                            // Turns the timer back on
    ~timer1();                                  // Resets timer1 to it's default values.
};

#include "timer1.cpp"
#endif
