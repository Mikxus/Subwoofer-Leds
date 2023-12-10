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

#include "SubEffects.h"

bool led_manager::update()
{
    sl_list::node<ledStrip> *led_strip_iter = led_strip_list.head();
    sl_list::node<audioMode> *audio_effect_node;
    bool update_state = 0;
    
    while (led_strip_iter != nullptr) // iterate over all ledstrips
    {
        audio_effect_node = led_strip_iter->data->effect_list.head();

        while(audio_effect_node != nullptr) // iterate over all audio_effects
        {
            if (audio_effect_node->data->update())
                update_state = 1;

            audio_effect_node = led_strip_iter->data->effect_list.next(audio_effect_node);
        }

        led_strip_iter = led_strip_list.next(led_strip_iter);
    }

    if (update_state)
        FastLED.show();

    return update_state;
}


bool led_manager::_add_ledstrip(
        sl_list::node<ledStrip> &ledstrip_node,
        CRGB *pixel_array,
        uint16_t array_size)
{
    if (pixel_array == nullptr)
    {
        ERROR("add_ledstrip: empty pixel_array");
        return 1;
    }

    ledstrip_node.data->led_rgb_data = pixel_array;
    ledstrip_node.data->led_rgb_data_size = array_size;

    led_strip_list.append(&ledstrip_node);
    INFO(F("Added ledstrip with rgb data size: "), array_size);
    return 0;
}

bool led_manager::add_led_strip(
        ledStrip &led_strip,
        CRGB *pixel_array,
        uint16_t array_size)
{
    return _add_ledstrip(
            led_strip.ledStrip_node,
            pixel_array,
            array_size);
}


/**
 * @brief 
 * 
 * @param ledstrip_node 
 * @param effect_node 
 * @param pixel_start 
 * @param pixel_end 
 * @return true 
 * @return false 
 */
bool led_manager::_add_effect(
    sl_list::node<ledStrip> &ledstrip_node,
    sl_list::node<audioMode> &effect_node,
    CRGB *pixel_start,
    CRGB *pixel_end)

{
    if (!pixel_start || !pixel_end)
    {
        ERROR(F("add_effect: nullpointer passed as pixel address"));
        ERROR(F("pixel_start: "), (uint16_t) pixel_start);
        ERROR(F("pixel_end: "), (uint16_t) pixel_end);
        return 1;
    }

    /* Add effect to list */
    ledstrip_node.data->effect_list.append(&effect_node);

    /* initialize effect */
    effect_node.data->init_values(pixel_start, pixel_end);

    #ifdef DEBUG_CHECKS
    INFO(F("add_effect: added effect"));
    Serial.print(F("ledstrip: 0x"));
    Serial.println((uint16_t) &ledstrip_node, HEX);
    Serial.print(F("effect_node: 0x"));
    Serial.println((uint16_t) &effect_node, HEX);
    #endif

    return 0;
}

bool led_manager::add_effect(
        ledStrip &led_strip,
        audioMode &audio_effect,
        CRGB *pixel_start,
        CRGB *pixel_end)
{
   return _add_effect(
        led_strip.ledStrip_node,
        audio_effect.get_node(),
        pixel_start,
        pixel_end); 
}


bool led_manager::add_effect(
        ledStrip &led_strip,
        audioMode &audio_effect)
{
    /* Set the effects pixel range as the whole strip */
    return _add_effect(
            led_strip.ledStrip_node,
            audio_effect.get_node(),
            &led_strip.led_rgb_data[0],
            &led_strip.led_rgb_data[led_strip.led_rgb_data_size]);
}