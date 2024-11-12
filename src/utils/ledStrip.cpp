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
 * @brief Add effect to led strip
 * 
 * @param effect_node 
 * @param pixel_start 
 * @param pixel_end 
 * @return true 
 * @return false 
 */
bool ledStrip::_add_effect(
        sl_list::node<audioMode> &effect_node,
        CRGB *pixel_start,
        CRGB *pixel_end)
{
    if (!pixel_start || !pixel_end)
    {
        ERROR(F("add_effect: nullpointer passed as pixel address"));
        return 1;
    }

    if (!effect_node.data)
    {
        ERROR(F("add_effect: effect_node doesn't contain data"));
        return 1;
    }

    if (effect_list.find(&effect_node))
    {
        ERROR(F("add_effect: effect already added"));
        return 1;
    }

    effect_list.append(&effect_node);

    effect_node.data->resize(pixel_start, pixel_end);

    #ifdef DEBUG_CHECKS
    INFO(F("add_effect: added effect"));
    Serial.print(F("ledstrip: 0x"));
    Serial.println((uint16_t) this, HEX);
    Serial.print(F("effect_node: 0x"));
    Serial.println((uint16_t) &effect_node, HEX);
    #endif

    return 0;
}

/**
 * @brief Add audio effect to led strip with defined pixel range
 * 
 * @param audio_effect 
 * @param pixel_start 
 * @param pixel_end 
 * @return true 
 * @return false 
 */
bool ledStrip::add_effect(
        audioMode &audio_effect,
        CRGB *pixel_start,
        CRGB *pixel_end)
{
    return _add_effect(
        audio_effect.get_node(),
        pixel_start,
        pixel_end);
}

/**
 * @brief Add audio effect to led strip with whole strip as pixel range
 * 
 * @param audio_effect 
 * @return true 
 * @return false 
 */
bool ledStrip::add_effect(audioMode &audio_effect)
{
    return _add_effect(
        audio_effect.get_node(),
        &led_rgb_data[0],
        &led_rgb_data[led_rgb_data_size]);
}

/**
 * @brief Removes the effect from led strip
 * 
 * @note Only removes the same object as the one passed as argument
 * 
 * @param audio_effect 
 * @return true if not found
 * @return false if succesfully removed
 */
bool ledStrip::remove_effect(audioMode &audio_effect)
{
    return effect_list.remove(&audio_effect.get_node());
}

/**
 * @brief Updates the led strip's effects
 * 
 * @return true 
 * @return false 
 */
bool ledStrip::update()
{
    sl_list::node<audioMode> *effect_node = effect_list.head();
    bool led_changed = 0;

    while (effect_node != nullptr)
    {
        if (effect_node->data == nullptr)
        {
            WARN(F("update: effect_node doesn't contain data"));
            effect_node = effect_list.next(effect_node);
            continue;
        }

        if (effect_node->data->update())
                led_changed = 1;

        effect_node = effect_list.next(effect_node);
    }

    return led_changed;
}

ledStrip::~ledStrip()
{
}
