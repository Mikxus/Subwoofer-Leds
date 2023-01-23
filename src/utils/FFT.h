/*
 * MIT License
 * 
 * Copyright (c) 2023 Mikko Johannes Heinänen 
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
#ifndef _FFT_H_
#define _FFT_H_

#include <inttypes.h>
#include "../config.h"
#include "debug.h"
#include "arch/avr/atmega328p/timer1.h"
#include "FFT_strategy.h"
#include "../lib/Fixed8FFT/Fixed8FFT.h"


class FFT
{
private:
    FFT_backend_template* fft = nullptr;
    void *interrupt_data = nullptr;
    static timer1 timer;

public:
    FFT( uint8_t input_pin, uint16_t sample_size, uint16_t frequency, fft_backend bits = fixed_8 )
    {
        switch( bits ) {
            case fixed_8:
                fft = new Fixed8FFT(input_pin, sample_size, frequency, bits); 
                break;
            
            default:
                ERROR(F("Invalid bits number"));

                #ifdef DEBUG_CHECKS
                if ( bits > fixed_8 ) ERROR(F("Bits number: "), bits, F(" isnt't implemented"));
                #endif
                return;
        }
        
        if (fft == nullptr)
        {
            ERROR(F("Not enough memory for fft backend: "), bits);
            return;
        }
        
        /* TODO: Seutup interrupt's for sampling */
        if (risr_get_vector(TIMER1_COMPB_) == fft->get_read_vector())
        {
            ERROR(F("FFH: Timer1 compb vector is already in use"));
            return;
        }

        cli();
        risr_bind(TIMER1_COMPB_, fft->get_read_vector());
        #ifdef DEBUG_CHECKS
            Serial.print(F("Function: 0x"));
            Serial.print(reinterpret_cast<long unsigned int>(fft->get_read_vector()) , HEX);
            Serial.println(F(" binded to TIMER1_COMPB interrupt"));
        #endif
        timer.Start(frequency);
        sei();
    }

    uint16_t calculate()
    {
        if (fft == nullptr)
        {
            ERROR(F("FFT: No FFT backend initialized"));
            return 0;
        }

        return fft->calculate();
    }
};


#endif