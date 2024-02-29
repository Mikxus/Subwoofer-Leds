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
#include "../utils/data_types/virtual_led_array.h"
#include "../utils/data_types/singly_linked_list.h"
#include "../utils/debug.h"
#include "../config.h"

struct ledStrip;

/**
 * @brief audioMode 
 * 
 */
class audioMode
{
    uint8_t id = 0;
    sl_list::node<audioMode> list_node = sl_list::node<audioMode>(this, nullptr);

protected:
    /* These are inherited */
    virtual_led_array led_array;
    CRGBPalette16 color_palette = CRGBPalette16(CRGB::Black);

public:

    /**
     * @brief Resize effect's led array 
     * 
     * @param array_start start address
     * @param array_end  end address
     * @param clear_old_area 1 to set all pixels to black on the old area. 
     */
    bool resize(CRGB *array_start, CRGB* array_end, bool clear_old_area = true)
    {
       return led_array.resize(array_start, array_end, clear_old_area);
    } 

    sl_list::node<audioMode> &get_node() {return list_node;}

    void set_color_palette(const TProgmemPalette16 &palette) {color_palette = palette;}

    /**
     * @brief 
     * @note function wich should be implemented in the inherited class the following way.
     *       updates the led strip's values. 
     *       Returns 1 if any led value changed. Othervise 0
     *       update shouldn't call FastLED.show() or any other function that updates the led strips
     * 
     * @return true When led value changed
     * @return false when no change happened
     */
    virtual bool update() = 0;

    audioMode() {}
    virtual ~audioMode() = default;
};
#endif
