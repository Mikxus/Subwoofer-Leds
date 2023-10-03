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

#include "ledStrip.h"

/**
 * @brief Sets brightness
 *
 * @param value
 */
void ledStrip::set_brightness(uint8_t value)
{
    brightness = value;
    return;
}

__attribute__((deprecated)) bool ledStrip::set_colorpalette(uint8_t index)
{
    if (index >= NUM_OF_PALETTES)
    {
        ERROR(F("ledstrip.cpp: Colorpalette index not found\n\rInput: "), index, F("\n\rNumber of palettes: "), NUM_OF_PALETTES);
        return 0;
    }

    palette_index = index;
    if (!set_color_palette(palette_index))
    {
        ERROR(F("ledstrip.cpp: Failed to set color palette\n\rIndex: "), palette_index, F("\n\rStrip id: "), identifier);
    }

    INFO(F("Colorpalette: "), palette_index, F(" set for strip: "), identifier, F("\n\r"));
    return 1;
}

__attribute__((deprecated)) bool ledStrip::SetMode(uint8_t modeIndex, CFastLED *fptr)
{
    if (modeIndex >= NUM_OF_MODES)
        return 0; // if invalid input return 0

    if (!load_mode(mode, fptr)) // check if fails
    {
        ERROR("ledStrip.cpp: Failed to set mode: ", modeIndex);
        return 0;
    }
    return 1;
}

__attribute__((deprecated)) bool ledStrip::load_mode(uint8_t modeIndex, CFastLED *fptr)
{
    // Check if CFastLED pointer is valid
    if (fptr == nullptr)
        return 0;

    // check if effect_ptr_head is already loaded
    if (effect_ptr_head != nullptr)
    {
        // unload mode
        INFO(F("Mode already loaded for strip: "), identifier, F(" unloading current mode:"), mode);
        unload_obj();
    }

    switch (modeIndex) // switch for the different modes
    {
    case 0: // colorBass mode

        effect_ptr_head = new colorBass();

        break;
    default:
        return 0;
        break;
    }

    if (effect_ptr_head == nullptr)
    {
        ERROR(F("ledStrip: Not enoug memory for mode: "), modeIndex);
        return 0;
    }

    // Share ledstrip & FastLED pointer with the new mode.
    effect_ptr_head->init_values(this, fptr);

    mode = modeIndex;
    INFO(F("Succesfully loaded mode: "), modeIndex, F(" for strip: "), identifier);
    return 1;
}

/**
 * @brief Adds audioMode to the selected ledstrip
 * 
 * @param ptr 
 * @param led_array_start 
 * @param led_array_end 
 * @return true 
 * @return false 
 */
bool ledStrip::add_effect(audioMode *ptr, CRGB *led_array_start, CRGB *led_array_end)
{
    if (effect_ptr_head == nullptr)
    {
        ERROR(F("ledStrip: Nullptr passed as audioMode"));
        return 0;
    }

    effect_ptr_head = ptr;
    effect_ptr_head->init_values(this, led_array_start, led_array_end);

    INFO(F("Succesfully loaded mode at an address: 0x"), reinterpret_cast<uint16_t>(ptr), F(" for strip: "), identifier);
    return 1;
}

__attribute__((deprecated)) bool ledStrip::set_color_palette(uint8_t colorIndex)
{
    switch (colorIndex) // add new case for your color palette. !color index can't jump any digits
    {
    case 0:
        colorPalette = blueBass_p;
        return 1;

    default:
        ERROR(F("ledStrip: color palette not found. index: "), colorIndex);
        return 0;
        break;
    }
    return 0;
}

ledStrip::~ledStrip()
{
    if (dynamically_allcated)
        free(led_rgb_data);
    return;
}
