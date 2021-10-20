#ifndef TIMER1_H
#define TIMER1_H


class timer1                                                     // Manages the arduino uno's timer1
{
    protected:

    void Start(uint16_t _size, uint16_t freq);
    void Stop();

    ~timer1();                                                  // Deconstructor
};

#include "timer1.cpp"
#endif

