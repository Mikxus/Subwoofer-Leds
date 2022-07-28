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

#include "timer1.h"

void timer1::SetPrescaler(uint16_t value)               // Reference: ATmega328p Datasheet          ! TCR1B register should be set to 0 before calling this function
{                                                       // https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
    switch (value)                                      // Page 110, Table 15-6    
    {
    case 1:                     // No prescaler
        //TCCR1B |= (0 << CS12);
        //TCCR1B |= (0 << CS11);
        TCCR1B |= (1 << CS10);
        break;
    case 8:                     // 1/8 prescaler
        //TCCR1B |= (0 << CS12);
        TCCR1B |= (1 << CS11);
        //TCCR1B |= (0 << CS10);
        break;

    case 64:                    // 1/64 prescaler
        //TCCR1B |= (0 << CS12);
        TCCR1B |= (1 << CS11);
        TCCR1B |= (1 << CS10);
        break;

    case 256:                   // 1/256 prescaler
        TCCR1B |= (1 << CS12);
        //TCCR1B |= (0 << CS11);
        //TCCR1B |= (0 << CS10);
        break;

    case 1024:                  // 1/1024 prescaler
        TCCR1B |= (1 << CS12);
        //TCCR1B |= (0 << CS11);
        TCCR1B |= (1 << CS10);
        break;

    default:
        #ifdef DEBUG
            Serial.print(F("Timer1: Incorrect prescaler value given: "));
            Serial.println(value);
            Serial.print(F("Correct values are: "));
            for (uint8_t i = 0; i < sizeof(prescalers); i++)
            {
                Serial.print(prescalers[i]);
                if (i != sizeof(prescalers) -1 ) Serial.println(F(", "));
            }
        #endif
        break;
    }
}

float timer1::GetTimerFrequency()
{
    return _achievedFrequency;
}

void timer1::SetTimerFrequency(uint32_t frequency)
{
    uint16_t OCR1A_value;
    uint8_t bestPrescaler;

    for (uint8_t i = 0; i < 4;i++)         // check for the most accurate prescaler. Prescaler is in laymen's terms a clock divider.
    {
        OCR1A_value = 16000000 / ( frequency * prescalers[i] ) - 1;
        if ( OCR1A_value <= uint16_t(-1))  // check if the value is under 16bit. The arduino uno's timer1 OCR1A register only supports 16 bit values
        {
            bestPrescaler =  i;
            break;                         // break since the smallest prescaler is the most accurate
        }
    }
    cli();
    SetPrescaler(prescalers[ bestPrescaler ]);
    OCR1A = OCR1A_value;
    sei();
    _achievedFrequency = 16000000 / prescalers[bestPrescaler] / OCR1A_value;
}

void timer1::Start(uint32_t freq)
{  
    uint32_t OCR1A_value;
    uint8_t bestPrescaler;

    for (uint8_t i = 0; i < 4;i++)         // check for the most accurate prescaler
    {
        OCR1A_value = 16000000 / ( freq * prescalers[i] ) - 1;
        
        if ( OCR1A_value <= uint16_t(-1))  // check if the value is under 16bit. The arduino uno's timer1 OCR1A register only supports 16 bit values
        {
            bestPrescaler =  i;
            break;                         // break since the smallest prescaler is the most accurate
        }
    }
    //Serial.print(F("New prescaler used: "));
    //Serial.println(prescalers[bestPrescaler]);
    cli();                                 // disable interrupts

    if (PRTIM1)                            // check if the timer is disabled
    {
        PRR &= ~( 1 << PRTIM1 );            // Enable timer1
    }
    
    TCCR1A = 0;                            // set entire TCCR1A register to 0
    TCCR1B = 0;                            // same for TCCR1B
    TCNT1  = 0;                            // initialize counter value to 0 

    OCR1A = (uint16_t) OCR1A_value;        // OCR1A = (16*10^6) / ( frequency * prescaler ) - 1 (must be under 16bit)
    TCCR1B |= (1 << WGM12);                // turn on CTC mode

    SetPrescaler(prescalers[bestPrescaler]); // Set prescaler

    TIMSK1 |= (1 << OCIE1B);               // enable timer1 compare B interrupt
    sei();                                 // enable interrupts
    
    _achievedFrequency = 16000000.0 / (float) prescalers[bestPrescaler] / (float) OCR1A_value;  // Calculate and save the frequency achieved
}

inline void timer1::Stop()
{
   PRR |= 0x4;                   // Turns timer1 off
}

inline void timer1::Continue()                    // Disables the power reduction
{
    PRR &= 0x7b;                          // turns timer1 on   // might be wrong number
}

timer1::~timer1()                           
{
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    OCR1A = 0;
    TIMSK1 &= 0x4;
}

volatile uint16_t _fftArrPos;
ISR(TIMER1_COMPB_vect)                              // Interrupt routine for the fft library. each iteration it checks if the sapling has completed. If not it saves the current analog input to the array
{
    if (!_fftBinReady)
    {
        uint16_t val = analogRead(_subwooferPin);
        if ( val <= _calibratedNoiseZero )          // check if the reading is above the noise level if it isn't sets the value to zero
        {
            val = 0;
        }
        if (_fftArrPos < _fftBinSize )              // if the _fftArrPos is within the array size save the value. else set the value to zero and set _fftBinReady to true
        {
            _vReal[_fftArrPos] = val;
            _fftArrPos += 1;
        } else
        {
            _fftArrPos = 0;
            _fftBinReady = true;
        }
        return;
    }
}