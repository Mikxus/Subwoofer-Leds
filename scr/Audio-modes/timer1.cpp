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

#include <Arduino.h>
#include "timer1.h"

#ifndef 

void timer1::SetPrescaler(uint16_t value)               // Reference: ATmega328p Datasheet
{                                                       // https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    switch (value)                                      // Page 110, Table 15-6    
    {
    case 0:         // No prescaler
        CS12 = 0;
        CS11 = 0;
        CS10 = 1;
        break;
    case 8:         // 1/8 prescaler
        CS12 = 0;
        CS11 = 1;
        CS10 = 0;
        break;

    case 64:         // 1/64 prescaler
        CS12 = 0;
        CS11 = 1;
        CS10 = 1;
        break;

    case 256:         // 1/256 prescaler
        CS12 = 1;
        CS11 = 0;
        CS10 = 0;
        break;

    case 1024:         // 1/1024 prescaler
        CS12 = 1;
        CS11 = 0;
        CS10 = 1;
        break;

    default:
        Serial.println(F("Not working"));       // for debug. will be deleted after testing.
        Serial.print(F("Value: "));
        Serial.println(value);
        break;
    }
}

void timer1::Start(uint16_t size, uint16_t freq)
{  
    uint16_t real_sample_rate;
    int32_t calcVal;
    int32_t bestVal = NULL;
    int32_t val;
    //int32_t difference;

    for (uint8_t i = 0; i < 5;i++)
    {
        calcVal = 16000000 / ( freq * prescalers[i] )
        val = 16000000 / prescalers[i] / caclVal;
        if (val <= 65,536)
        {
            if ( abs( bestVal - val > val) ) bestVal = val;
        }
    }
    real_sample_rate = 16000000 / (freq * size) -1;

    cli();
    if (PRTIM1)                           // check if the timer is disabled
    {
        #ifdef DEBUG
        sei();
        Serial.print(F("Timer1 is disabled | Turning it on"));
        cli();
        #endif
        PRTIM1 = 0;
    {
    
                                           // set timer1 interrupt at 1kHz
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            // initialize counter value to 0 
                                           // OCR1A = real_sample_rate; // = (16*10^6) / (2000*64) - 1 (must be under 16bit) set compare match register for 1khz increments
    OCR1A = real_sample_rate;
    TCCR1B |= (1 << WGM12);                // turn on CTC mode
    //SetPrescaler();
    TCCR1B |= (1 << CS11) | (1 << CS10);   // Set CS11 and CS10 bits for 64 prescaler
    TIMSK1 = (1 << OCIE1B);                // enable timer1 compare B interrupt
    sei();
}

void timer1::Stop()
{
    /*
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            // initialize counter value to 0
    */
   PRTIM1 = 1;                              // Power reduction for timer1. in simple terms it gets turned off.
}

void timer1::Continue()                     // Disables the power reduction
{
    PRTIM1 = 0;
}

timer1::~timer1()
{
    Stop();
}