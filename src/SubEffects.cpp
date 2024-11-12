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
    bool update_state = 0;
    
    /* iterate over all ledstrips */
    while (led_strip_iter != nullptr)
    {
        if (led_strip_iter->data == nullptr)
        {
            ERROR("update: led_strip_iter->data is nullptr");
            led_strip_iter = led_strip_list.next(led_strip_iter);
            continue;
        }

        /* Update effects on the led strip */
        if (led_strip_iter->data->update())
            update_state = 1;

        led_strip_iter = led_strip_list.next(led_strip_iter);
    }

    if (update_state)
        FastLED.show();

    return update_state;
}


bool led_manager::_add_ledstrip(
        sl_list::node<ledStrip> &ledstrip_node,
        CRGB *pixel_array,
        const uint16_t array_size)
{
    if (pixel_array == nullptr)
    {
        ERROR("add_ledstrip: empty pixel_array");
        return 1;
    }

    ledstrip_node.data->led_rgb_data = pixel_array;
    ledstrip_node.data->led_rgb_data_size = array_size;

    led_strip_list.append(&ledstrip_node);
    INFO(F("Added ledstrip with: "), array_size, F(" leds"));
    return 0;
}


/**
 * @brief Add led strip to led manager
 * 
 * @param led_strip 
 * @param pixel_array
 * @param array_size
 * @return true 
 * @return false 
 */
bool led_manager::add_led_strip(
        ledStrip &led_strip,
        CRGB *pixel_array,
        const uint16_t array_size)
{
    return _add_ledstrip(
            led_strip.ledStrip_node,
            pixel_array,
            array_size);
}

/**
 * @brief Add audio effect to led strip with defined pixel range
 * 
 * @param led_strip 
 * @param audio_effect 
 * @param pixel_start 
 * @param pixel_end 
 * @return true 
 * @return false 
 */
bool led_manager::add_effect(
        ledStrip &led_strip,
        audioMode &audio_effect,
        CRGB *pixel_start,
        CRGB *pixel_end)
{
   return led_strip.add_effect(
        audio_effect,
        pixel_start,
        pixel_end); 
}

/**
 * @brief Add audio effect to led strip with whole strip as pixel range 
 * 
 * @param led_strip 
 * @param audio_effect 
 * @return true 
 * @return false 
 */
bool led_manager::add_effect(
        ledStrip &led_strip,
        audioMode &audio_effect)
{
    return led_strip.add_effect(audio_effect);
}

/**
 * @brief Removes the effect from led strip
 * 
 * @param ledstrip 
 * @param audio_effect 
 * @return true if effect not found
 * @return false if succesfully removed 
 */
bool led_manager::remove_effect(
    ledStrip &ledstrip,
    audioMode &audio_effect)
{
    return ledstrip.remove_effect(audio_effect);
}

/**
 * @brief Searches all led strips until effect is found and removes it
 * @note doesn't remove from all ledstrips since effect shouldn't  be able to be in multiple ledstrips
 * @param audio_effect 
 * @return true 
 * @return false 
 */
bool led_manager::remove_effect(audioMode &audio_effect)
{
    sl_list::node<ledStrip> *led_strip_iter = led_strip_list.head();

    while(led_strip_iter != nullptr)
    {
        if (led_strip_iter->data == nullptr) break;

        if (!led_strip_iter->data->remove_effect(audio_effect))
            return 0;

        led_strip_iter = led_strip_list.next(led_strip_iter);
    }

    /* Not found */
    return 1;
}