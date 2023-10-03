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

/**
 * @brief Construct a new Sub Effects:: Sub Effects object
 *
 */
SubEffects::SubEffects()
{
}

/**
 * @brief Returns the position of the led strip pointer in
 *        _strips array
 *
 * @param identifier    id of the led strip
 * @return uint8_t
 */
uint8_t SubEffects::GetStripPos(uint8_t identifier)
{
    for (uint8_t i = 0; i < _stripsPos; i++)
    {
        if (_strips[i]->identifier == identifier)
            return i;
    }

    ERROR(F("SubEffects: Strip not found: "), identifier);
    return 255;
}

/**
 * @brief Returns pointer to the led strip if matching identifier found
 *
 * @param identifier    Id of the led strip
 * @return ledStrip*
 */
ledStrip *SubEffects::GetStripPtr(uint8_t identifier)
{
    for (uint8_t i = 0; i < MAX_STRIPS; i++)
    {
        if (_strips[i]->identifier == identifier)
        {
            return _strips[i];
        }
    }

    WARN("GetStripPtr: Strip with id of: ", identifier, " not found\n");
    return nullptr;
}

/**
 * @brief Updates matching led strip's parameters.
 *        like mode and colorpalette
 *
 * @param identifier    Id of the led
 * @return bool
 */
bool SubEffects::UpdateStripValues(uint8_t identifier)
{
    ledStrip *ptr = GetStripPtr(identifier);
    if (ptr == nullptr)
        return 0;

    if (ptr->mode != _currentMode)
    {
        if (!_strips[i]->SetMode(_currentMode, &FastLED))
            isFailure = 1;
    }

    if (_strips[i]->palette_index != _currentPalette)
    {
        if (!ptr->set_colorpalette(_currentPalette))
            return 0; // fail
    }

    return 1;
}

/**
 * @brief Updates all led strips
 * @return None
 */
void SubEffects::UpdateAllStripsValues()
{
    for (uint8_t i = 0; i < _stripsPos; i++)
    {
        UpdateStripValues(_strips[i]->identifier);
    }
    return;
}

/**
 * @brief Returns matching led strip's led array pointer
 *
 * @param identifier    Led strip's identifier
 * @return CRGB*        Pointer to leds array if found.
 */
CRGB *SubEffects::GetLedsPtr(uint8_t identifier)
{
    for (uint8_t i = 0; i < _stripsPos; i++)
    {
        if (_strips[i]->identifier == identifier)
            return _strips[i]->led_rgb_data;
    }
    ERROR(F("SubEffects: ledStrip not found with ID: "), identifier);
    return nullptr; // return nullptr if the led wasn't found
}

/**
 * @brief Adds new led strip & automatically allocates
 *        new CRGB led array
 * @note Uses dynamic allocation for the led array
 *
 * @param audioPin      Analog pin number
 * @param mode          Number of the wanted led mode
 * @param ledArrSize    Number of leds
 * @return uint8_t
 */
uint8_t SubEffects::add_strip(uint8_t audioPin, uint8_t mode, uint16_t ledArrSize)
{
    CRGB *ledsPtr;
    ledsPtr = (struct CRGB *)calloc(ledArrSize, sizeof(struct CRGB));
    uint8_t state = 0;

    if (ledsPtr == nullptr) // Check if allocation failed
    {
        ERROR(F("Failed to allocate space for leds data array"));
        return 0;
    }

    state = add_strip(audioPin, mode, ledArrSize, ledsPtr);

    if (!state) // Check if AddledStrip() fails
    {
        delete ledsPtr; // free allocated space
        return 0;
    }
    return state; // returns identifier for the strip                                                        // Otherwise return 1;
}

/**
 * @brief Adds new led strip & initializes it for use.
 *
 * @param audioPin      Analog pin number
 * @param ledMode       Number of the wanted ledmode
 * @param ledArrSize    Size of the ledArray
 * @param ledArray      Pointer of the ledArray
 * @return uint8_t
 */
uint8_t SubEffects::add_strip(uint8_t audioPin, uint8_t ledMode, uint16_t ledArrSize, CRGB *ledArray)
{
    if (_stripsPos >= MAX_STRIPS) // Check if there is space for new strip. if there isn't return 0
    {
        WARN(F("SubEffects: Max number of led strips reached\n"));
        return 0;
    }

    if (_strips[_stripsPos] != nullptr)
        return 0;

    _strips[_stripsPos] = new ledStrip;
    if (_strips[_stripsPos] == nullptr)
    {
        ERROR(F("SubEffects: Failed to allocate space for new led struct\n"));
        return 0;
    }
    /* Assing values to strip struct  */
    _strips[_stripsPos]->led_rgb_data = ledArray;
    _strips[_stripsPos]->led_rgb_data_size = ledArrSize;
    _strips[_stripsPos]->mode = ledMode;
    _strips[_stripsPos]->inputPin = audioPin;
    _strips[_stripsPos]->identifier = _stripsPos + 1; // increase _stripsPos
    _strips[_stripsPos]->SetMode(ledMode, _fastPtr);
    _strips[_stripsPos]->set_colorpalette(_currentPalette);

    _stripsPos++;
    /* Return strip's idenifier */
    return _stripsPos;
}

/**
 * @brief Removes all created led strips
 * @return None
 */
void SubEffects::remove_all_strips()
{
    for (uint8_t i = 0; i < MAX_STRIPS; i++)
    {
        if (_strips[i] != nullptr)
            delete _strips[i];
    }
    _stripsPos = 0;
    return;
}

bool SubEffects::remove_strip(CRGB *ledArray) // Removes the led strip which holds the given CRGB* object
{
    for (uint8_t i = 0; i < _stripsPos; i++) // iterate over all of the "allocated" objects
    {
        if (_strips[i]->led_rgb_data == ledArray) // if match
        {
            if (remove_strip(_strips[i]->identifier))
                return 1;
        }
    }

    ERROR(F("SubEffects: Strip not found"));
    return 0;
}

/**
 * @brief Removes led strip with the matching identifier
 *
 * @param identifier    Id of the led strip
 * @return true
 * @return false
 */
bool SubEffects::remove_strip(uint8_t identifier)
{
    uint8_t i = 0;

    i = GetStripPos(identifier);
    if (i == 255)
        return 0; // Strip not found

    delete _strips[i]; // delete it
    _strips[i] = nullptr;

    if (i < _stripsPos - 1) // Check if the strips isn't the last one
    {
        for (uint8_t a = i; a < _stripsPos; a++) //  then sift all the remaining elements down by 1
        {
            _strips[a] = _strips[a + 1];
            if (a == MAX_STRIPS)
                _strips[a] = nullptr; // set the last object of the _strips array to nullptr
        }
    }

    _stripsPos--;
    return 1;
}

/**
 * @brief Calibrates the zero value for all led strips
 * @note Takes 1.5s to complete
 *
 */
void SubEffects::CalibrateNoise()
{
    for (uint8_t i = 0; i < _stripsPos; i++) // Set all strips calibration values to 0
    {
        _strips[i]->inputCalZero = 0;
    }

    uint16_t CalibrateTime = millis();

    while (millis() < CalibrateTime + 1500)
    {

        for (uint8_t i = 0; i < _stripsPos; i++)
        {
            uint16_t val = 0;
            val = analogRead(_strips[i]->inputPin);
            if (val > _strips[i]->inputCalZero)
                _strips[i]->inputCalZero = val + 1;
        }
    }

#ifdef DEBUG_CHECKS
    for (uint8_t i = 0; i < _stripsPos; i++)
    {
        INFO(F("Strip: "), _strips[i]->identifier, F(" input pin: "), _strips[i]->inputPin, F(" Calibration value: "), _strips[i]->inputCalZero);
    }
#endif
    return;
}

/**
 * @brief Sets the global brightness
 *
 * @param brightness
 */
void SubEffects::set_brightness(uint8_t brightness)
{
    _brightValue = brightness;
    return;
}
/**
 * @brief Increases the global brightness
 *
 * @return true
 * @return false
 */
bool SubEffects::increase_brightness()
{
    if (_brightValue != 255)
    {
        _brightValue++;
        return 1;
    }
    return 0;
}

/**
 * @brief Decreases the global brightness
 *
 * @return true
 * @return false
 */
bool SubEffects::decrease_brightness()
{
    if (_brightValue != 0)
    {
        _brightValue--;
        return 1;
    }
    return 0;
}

/**
 * @brief Calls each led strip's "effect"
 *
 */
void SubEffects::update()
{
    bool change = 0;
    for (uint8_t i = 0; i < _stripsPos; i++)
    {
        if (_strips[i]->effect_ptr_head == nullptr)
        {
            DEBUG(F("SubEffects: Mode not set for strip with id: "), _strips[i]->identifier);
            continue; // if mode isn't set for strip, skip it.
        }
        if (_strips[i]->effect_ptr_head->update())
            change = 1;
    }

    // if there was anything changed show the leds
    if (change)
        FastLED.show();
    return;
}

/**
 * @brief Increases current global led mode by 1
 * @note Updates all leds to the current global led mode
 *
 */
__attribute__((deprecated)) void SubEffects::NextMode()
{
    if (_currentMode + 1 < NUM_OF_MODES)
        _currentMode++;
    else
        _currentMode = 0;

    UpdateAllStripsValues();
}

/**
 * @brief Decreases current global led mode by 1
 * @note Updates all leds to the current global led mode
 *
 */
__attribute__((deprecated)) void SubEffects::PreviousMode()
{
    if (_currentMode - 1 < NUM_OF_MODES)
        _currentMode--; // if variable mode - 1 is 0 set the variable to the max amount of "modes"
    else
        _currentMode = NUM_OF_MODES;

    UpdateAllStripsValues();
}

/**
 * @brief Sets all leds strips to specified mode
 *
 * @param wantedMode
 * @return None
 */
__attribute__((deprecated)) void SubEffects::SetMode(uint8_t wantedMode)
{
    if (wantedMode < NUM_OF_MODES)
    {
        _currentMode = wantedMode;
    }

    UpdateAllStripsValues();
    return;
}

bool SubEffects::load_mode(audioMode *ptr, uint8_t identifier, CRGB *led_array_start, CRGB *led_array_end)

{
    ledStrip *strip = GetStripPtr(identifier);
    if (strip == nullptr)
    {
        ERROR(F("LoadMode failed: Invalid identifier "), identifier);
        return 0;
    }

    strip->add_effect(ptr, led_array_start, led_array_end);
    return 1;
}

/**
 * @brief Incresses global color palette and sets all leds strips to it
 * @return None
 */
__attribute__((deprecated)) void SubEffects::next_color()
{
    if (_currentPalette + 1 > NUM_OF_PALETTES)
        _currentPalette = 0;
    else
        _currentPalette++;

    UpdateAllStripsValues();
    return;
}

/**
 * @brief Incresses global color palette and sets all leds strips to it
 * @return bool
 */
__attribute__((deprecated)) bool SubEffects::set_colorpalette(uint8_t palette_index)
{
    if (palette_index > NUM_OF_PALETTES)
        return 0;
    _currentPalette = palette_index;

    UpdateAllStripsValues(); // Updates all strips with the new color palette
    return 1;
}

/**
 * @brief Destroy the Sub Effects:: Sub Effects object
 *
 */
SubEffects::~SubEffects()
{
    remove_all_strips();
    return;
}