/*
 * MIT License
 * 
 * Copyright (c) 2022 Mikko Johannes Heinänen 
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


#ifndef _AUDIO_MODES_H_
#define _AUDIO_MODES_H_

#include <FastLED.h>

struct ledStrip;

class audioMode                         // mode template
{
protected:

    /* These are inherited */

    ledStrip *_ledStrip;                // Pointer for the led strip struct
    CFastLED *_FastLED;                 // Pointer for the current FastLED object

public:
    void initValues(ledStrip *ledStrip, CFastLED *FastLED) { _ledStrip = ledStrip; _FastLED = FastLED; }
    
    virtual bool Update() = 0;          // function wich should be implemented in the inherited class the following way.
                                        // updates the led strip's values. 
                                        // Returns 1 if any led value changed. Othervise 0
                                        // update shouldn't call FastLED.show() or any other function that updates the led strips
    virtual ~audioMode() = default;
};


#endif
