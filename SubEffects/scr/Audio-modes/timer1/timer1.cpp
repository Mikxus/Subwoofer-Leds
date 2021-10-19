#include <Arduino.h>
#include "timer1.h"

void timer1::Start(uint16_t size, uint16_t freq)
{  
    uint16_t real_sample_rate;
    real_sample_rate = 16000000 / (freq * size) -1;
    cli();
                                            //set timer1 interrupt at 1kHz
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0 
                                            //OCR1A = real_sample_rate; // = (16*10^6) / (2000*64) - 1 (must be under 16bit) set compare match register for 1khz increments
    OCR1A = real_sample_rate;
    TCCR1B |= (1 << WGM12);                // turn on CTC mode
    TCCR1B |= (1 << CS11) | (1 << CS10);   // Set CS11 and CS10 bits for 64 prescaler
    TIMSK1 = (1 << OCIE1B);                // enable timer1 compare B interrupt
    sei();
}

void timer1::Stop()
{
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            //initialize counter value to 0
}

timer1::~timer1 ()
{
    Stop();
}