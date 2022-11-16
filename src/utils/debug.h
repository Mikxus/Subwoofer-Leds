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
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <Arduino.h>
#include <inttypes.h>
#include "config.h"

typedef enum {
    no_newline,
    newline,
    newline_args
} newline_t;

/**
 * @brief High-intensity colors
 * @note https://i.stack.imgur.com/KTSQa.png
 * 
 */
typedef enum {
    GREY = 8,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    PINK,
    CYAN,
    WHITE
} terminal_colors;

typedef enum {
    error = CONF_ERROR_COLOR,
    warn = CONF_WARN_COLOR,
    info = CONF_INFO_COLOR,
    debug = CONF_DEBUG_COLOR,
} debug_level_t;

/* Easily change debug output*/
#define DEBUG_OUT(args) \
    Serial.print(args); \

/* Same as DEBUG_OUT, but without double colon ; */
#define DEBUG_OUT_NC(args) \
    Serial.print(args)

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wnarrowing"

/**
 * @brief Sets terminal to debug_level_t color
 * 
 * @param level     "error level"
 * @param reset_color when 1 restores terminal color to default
 */
void set_terminal_color( debug_level_t level, bool reset_color = 0 );

template <class ...Args> void debug_print( debug_level_t debug_level, newline_t newline , Args&... arg )
{
    set_terminal_color(debug_level);

#if CONF_APPEND_DEBUG_LEVEL == 1
    switch (debug_level)
    {
    case debug:
        DEBUG_OUT(F("DEBUG: "));
        break;

    case info:
        DEBUG_OUT(F("INFO: "));
        break;

    case warn:
        DEBUG_OUT(F("WARNING: "));
        break;

    case error:
        DEBUG_OUT(F("ERROR: ")); 
        break;
    }
#endif    
    /* My eyes */
    using expand_t = uint8_t[];
    expand_t {0,(DEBUG_OUT_NC(arg),0)...};
    set_terminal_color(debug_level, 1);

    if (newline) DEBUG_OUT(F("\n\r"));

    return;
}

template <class ...T> void DEBUG( [[maybe_unused]] const T&... debug_text ) 
{
    #if CONF_ENABLE_DEBUG == 1
    debug_print( debug, newline, debug_text...);
    #endif
    return;
}

template <class ...T> void ERROR( [[maybe_unused]] const T&... debug_text ) 
{
    #if CONF_ENABLE_ERROR == 1
    debug_print( error, newline, debug_text...);
    #endif
    return;
}

template <class ...T> void INFO( [[maybe_unused]] const T&... debug_text ) 
{
    #if CONF_ENABLE_INFO == 1
    debug_print( info, newline, debug_text...);
    #endif
    return;
}

template <class ...T> void WARN( [[maybe_unused]] const T&... debug_text ) 
{
    #if CONF_ENABLE_WARN == 1
    debug_print( warn, newline, debug_text...);
    #endif
    return;
}
#pragma GCC diagnostic pop
#endif