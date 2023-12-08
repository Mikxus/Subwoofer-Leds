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
#ifndef _SUBEFFECTS_LEDSTRIP_H_
#define _SUBEFFECTS_LEDSTRIP_H_

#include <FastLED.h>
#include <inttypes.h>
#include "../config.h"
#include "../colorPalettes.h"
#include "../Audio-modes/audioModes.h"
#include "virtual_led_array.h"
#include "singly_linked_list.h"
#include "debug.h"

/**
 * @brief Struct which contains info about a ledstrip
 * 
 */
struct ledStrip
{
    sl_list::node<ledStrip> ledStrip_node = sl_list::node<ledStrip>(this, nullptr);
    sl_list::handler<audioMode> effect_list;

    CRGB *led_rgb_data = nullptr;    // Pointer for the led array
    uint16_t led_rgb_data_size = 0;  // Number of leds on a single strip

    ledStrip() {}
    ~ledStrip();
};
#include "../Audio-modes/colorBass.h"
#endif