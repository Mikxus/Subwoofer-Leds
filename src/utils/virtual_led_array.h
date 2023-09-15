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
 *      Makes it easier to set effects to use arbitary pixels as their output
 *      Enabling multiple effects to work on 1 single led strip
 *
 */
struct virtual_led_strip_rgb_array
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

        if (start < end)
        { 
            ERROR("rgb_array_start has to be smaller than rgb_array_end");
            return 1;
        }

        data_array_start = start;
        data_array_end = end;
        return 0;
    }

    /* Array access operator */
    inline uint8_t operator[] (uint16_t x) const __attribute__((always_inline))
    {
        if (data_array_start + x * sizeof(CRGB) > data_array_end) 
            return 0;

        return data_array_start[x];
    }

    /* Assignment from other virtual thingy */
    inline virtual_led_strip_rgb_array& operator= (const virtual_led_strip_rgb_array& x) __attribute__((always_inline)) = default;

    /* Assignment from CRGB struct to virtual thingy */
    inline virtual_led_strip_rgb_array& operator= (const CRGB& x) __attribute__((always_inline))
    {
        *this = x;
        return *this;
    }

    virtual_led_strip_rgb_array(CRGB *rgb_array_start, CRGB *rgb_array_end)
    {
        resize(rgb_array_start, rgb_array_end);
    }

    /**
     * @brief Deconstructor. Sets the rgb values to black before exiting
     * 
     */
    ~virtual_led_strip_rgb_array()
    {
        CRGB *head = data_array_start;
        while (head == data_array_end)
        {
            (*head) = CRGB::Black;
        }
    }

private:
    CRGB *data_array_start = nullptr;
    CRGB *data_array_end = nullptr;
};

#endif