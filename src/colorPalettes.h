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

#ifndef _COLORPALETTES_H_
#define _COLORPALETTES_H_

const TProgmemPalette16 red_blue_pattern_p PROGMEM = 
{
    CRGB::Red,   // light blue
    CRGB::Red,
    CRGB::Red,
    CRGB::Red,

    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,   // dark blue
    CRGB::Blue,

    CRGB::Red,   // light blue
    CRGB::Red,
    CRGB::Red,
    CRGB::Red,

    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,   // dark blue
    CRGB::Blue,

};
extern const TProgmemPalette16 red_blue_pattern_p PROGMEM;

const TProgmemPalette16 blueBass_p PROGMEM = {
    CRGB::Red,   // light blue
    CRGB::Red,
    CRGB::Red,
    CRGB::Blue,

    CRGB::Cyan,
    CRGB::Green,
    CRGB::Yellow,
    CRGB::Yellow,

    CRGB::Green,   // light blue
    CRGB::Green,
    CRGB::Green,
    CRGB::Green,

    CRGB::Yellow,
    CRGB::Yellow,
    CRGB::Yellow,   // dark blue
    CRGB::Yellow,


};

extern const TProgmemPalette16 blueBass_p PROGMEM;

#endif