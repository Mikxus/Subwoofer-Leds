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
 * @brief singly linked list for ledStrip
 * 
 */
struct singly_link_ledStrip : public singly_link_base
{
    ledStrip* next_ledStrip = nullptr;
}

/**
 * @brief Struct which contains info about a ledstrip
 * 
 */
struct ledStrip : public singly_link_ledStrip
{
    audioMode *list_head = nullptr;
    CRGB *led_rgb_data = nullptr;    // Pointer for the led array
    uint16_t led_rgb_data_size = 0;  // Number of leds on a single strip


    uint8_t inputPin = 0;      // input pin used for the led srip
    uint16_t inputCalZero = 0; // Min value above the noise level
    uint16_t inputCalMax = 0;  // Max input value


    /* Methods for controlling the strip */
    void set_brightness(uint8_t value);
    bool set_colorpalette(uint8_t index);

    bool add_effect(audioMode *ptr, CRGB *led_array_start, CRGB *led_array_end);
    bool remove_effect(audioMode* ptr);

    ledStrip() = default;
    ~ledStrip();

    /* Deprecated stuff */
    __attribute__((deprecated)) uint8_t identifier = 0;   // value to associate strip later
    __attribute__((deprecated)) uint8_t mode = 0;         // contains light mode index
    __attribute__((deprecated)) uint8_t brightness = 255; // brightness scale     255 is equal to 100% of the led's brightness
    __attribute__((deprecated)) uint8_t palette_index = 0; // current palette id
    __attribute__((deprecated)) audioMode *effect_ptr_head = nullptr; // Pointer to audio mode class
    __attribute__((deprecated)) CRGBPalette16 colorPalette = CRGBPalette16(CRGB::Black);

    __attribute__((deprecated)) bool SetMode(uint8_t modeIndex, CFastLED *fptr);

protected:
    /* Pointer to next ledStrip object */
    ledStrip *next = nullptr;

    __attribute__((deprecated)) bool dynamically_allcated = 0; // true if CRGB *led_rgb_data is allocated by the ledStrip struct
    __attribute__((deprecated)) bool set_color_palette(uint8_t colorIndex);
    __attribute__((deprecated)) bool load_mode(uint8_t modeIndex, CFastLED *fptr); // "assings" mode to given audioMode object
};
#include "../Audio-modes/colorBass.h"
#endif