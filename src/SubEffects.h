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

#ifndef SUBEFFECTS_H
#define SUBEFFECTS_H

#if defined(__AVR_ATmega328P__)
#define _SUBEFFECTS_AVR_ATMEGA328_
#else
#error This library currently only supports AVR ATmega328 chip;      // comment out if you would still like to continue + remember to define  _SUBEFFECTS_AVR_ATMEGA328P_
#endif

#include <Arduino.h>
#include <inttypes.h>
#include <FastLED.h>
#include "config.h"
#include "utils/debug.h"
#include "utils/colorMath.h"
#include "utils/ledStrip.h"
#include "utils/interrupt.h"
#include "utils/virtual_led_array.h"
#include "Audio-modes/audioModes.h"

#include "Audio-modes/colorBass.h"

class led_manager
{
private:
    sl_list::handler<ledStrip> led_strip_list;
    
    bool _add_ledstrip(
        sl_list::node<ledStrip> &ledstrip_node,
        CRGB *pixel_array,
        const uint16_t array_size);

public:

    bool update();

    bool add_led_strip(
        ledStrip &led_strip,
        CRGB *pixel_array,
        const uint16_t array_size);

    bool add_effect(
        ledStrip &led_strip,
        audioMode &audio_effect,
        CRGB *pixel_start,
        CRGB *pixel_end);

    bool add_effect(
        ledStrip &ledstrip,
        audioMode &effect_node);


    bool remove_effect(
        ledStrip &ledstrip,
        audioMode &audio_effect);

    bool remove_effect(audioMode &audio_effect);
};

#endif
