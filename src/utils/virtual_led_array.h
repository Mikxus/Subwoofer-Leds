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

#ifndef _VIRTUAL_LED_ARRAY_H_
#define _VIRTUAL_LED_ARRAY_H_

#include <FastLED.h>
#include "config.h"
#include "debug.h"

/**
 * @brief "virtual" CRGB array
 *      Makes it easier to set effects to use arbitary pixels as their output range.
 *      This makes it possible to use multiple effects on a single output channel.
 *
 */
struct virtual_led_array
{
    /**
     * @brief Resizes the virtual CRGB pixel array
     * 
     * @param start Start address of the array
     * @param end  End address of the array
     * @return true 
     * @return false 
     */
    bool resize(CRGB* start, CRGB *end)
    {
        if (start == nullptr || end == nullptr)
        {
            ERROR("Nullpointer was passed as rgb array address");
            return 1;
        }

        if (start > end)
        { 
            ERROR("rgb_array_start has to be smaller than rgb_array_end");
            return 1;
        }

        data_array_start = start;
        data_array_end = end;
        return 0;
    }

    /**
     * @brief Returns the size of the virtual array's section
     * 
     * @return uint16_t size
     */
    __attribute((always_inline)) uint16_t size() {return ((uint16_t) data_array_end - (uint16_t) data_array_start) / sizeof(CRGB);}

    /**
     * @brief Array access operator to index into the virtual array
     * 
     * @param i The index to access
     * @return CRGB& Reference to the element at the specified index
     */
    inline CRGB& operator[] (uint16_t i) __attribute__((always_inline))
    {
        #ifdef DEBUG_CHECKS
        if (data_array_start + i * sizeof(CRGB) > data_array_end) 
        {
            WARN(F("virtual_array: Array overflow"));
            return data_array_start[0];
        }
        #endif

        return data_array_start[i];
    }

    /**
     * @brief Construct a new virtual led array object
     * 
     */
    virtual_led_array() = default;

    /**
     * @brief Deconstructor. Sets the rgb values to black before exiting
     * 
     */
    ~virtual_led_array()
    {
        CRGB *ptr = data_array_start;
        while (ptr == data_array_end)
        {
            (*ptr) = CRGB::Black;
            ptr++;
        }
    }

private:
    CRGB *data_array_start = nullptr;
    CRGB *data_array_end = nullptr;
};

#endif