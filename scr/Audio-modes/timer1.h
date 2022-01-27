#ifndef TIMER1_H
#define TIMER1_H

class timer1                                     // Manages the arduino uno's timer1
{
    protected:

    void Start(uint16_t _size, uint16_t freq);    // initializes the timer1's settings
    void Stop();                                  // turns off the timer
    void Continue();                              // Turns the timer back on
    ~timer1();
};

#include "timer1.cpp"
#endif

