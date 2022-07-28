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
#define DEBUG

void ledStrip::SetBrightness(uint8_t value)
{
    brightness = value;
}

bool ledStrip::SetColor(uint8_t index)
{
    if (index >= NUM_OF_PALETTES ) return 0;

    paletteIndex = index;
    if (!SetColorPalette(paletteIndex))
    {
        #ifdef DEBUG
            Serial.print(F("Failed to set color palette: "));
            Serial.print(paletteIndex);
            Serial.print(F(" | Strip id: "));
            Serial.println(identifier);
            Serial.flush();
            digitalWrite(13,HIGH);

        #endif
        return 0;
    }

    #ifdef DEBUG
        Serial.print(F("Color palette: "));
        Serial.print( paletteIndex);
        Serial.print(F(" set for strip: "));
        Serial.println( this->identifier );
    #endif


    return 1;
}

bool ledStrip::SetMode(uint8_t modeIndex, CFastLED * fptr)
{
    if ( modeIndex >= NUM_OF_MODES) return 0;       // if invalid input return 0

    if ( !loadMode( mode, fptr) )                          // check if fails
    {
        #ifdef DEBUG
            Serial.println(F("LedStrip: Failed to set mode"));
            digitalWrite(13,HIGH);
        #endif

        return 0;
    }
    return 1;
}


bool ledStrip::loadMode( uint8_t modeIndex, CFastLED *fptr)
{
    // Check if CFastLED pointer is valid
    if (fptr == nullptr) return 0;

    // check if modeUpdatePtr is already loaded

    if (modeUpdatePtr != nullptr)
    {
        // unload mode
        unloadMode();
    }

    switch (modeIndex)       // switch for the different modes
    {
        case 0:              // colorBass mode
            
            modeUpdatePtr = new colorBass();
            
            break;

        default:
            return 0;
            break;
    }

    if (modeUpdatePtr == nullptr) 
    {
        #ifdef DEBUG
            Serial.print(F("audioMode: Not enough memory for mode: "));
            Serial.println(modeIndex);
            digitalWrite(13,HIGH);
        #endif
        return 0;   
    }

    // Share ledstrip & FastLED pointer with the new mode.
    modeUpdatePtr->initValues(this, fptr);

    mode = modeIndex;
    
    return 1;   // sucessfull load
}

void ledStrip::unloadMode()
{
    if (modeUpdatePtr == nullptr) return;

    delete modeUpdatePtr;
    modeUpdatePtr = nullptr;
}

bool ledStrip::SetColorPalette(uint8_t colorIndex)
{
    switch (colorIndex)                         // add new case for your color palette. !color index can't jump any digits
    {
        case 0:
            colorPalette = blueBass_p;
            return 1;
        


        default:
            #ifdef DEBUG
                Serial.print(F("ledStrip: color palette not found. index: "));
                Serial.println(colorIndex);
                Serial.flush();
                digitalWrite(13,HIGH);
            #endif
            return 0;
            break;
    }
    return 0;
}


ledStrip::~ledStrip()
{
    unloadMode();
    if (dynAllocated) free (ledArr);
    
}