#include "debug.h"

void set_terminal_color( debug_level_t level, bool reset_color = 0 )
{   
    #if CONF_ENABLE_TERMINAL_COLOR == 1
    if (reset_color)
    {
        DEBUG_OUT(F("\033[39m\033[49m"));
        return;
    }

    DEBUG_OUT(F("\033[38;5;"));
    DEBUG_OUT(level);
    DEBUG_OUT(F("m"));
    #endif
    return;
}