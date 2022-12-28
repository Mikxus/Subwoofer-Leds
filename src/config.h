/* Definitions */

#define DEBUG_CHECKS                     // Uncomment to enable additional debug checks

#define MAX_STRIPS 2                // Max led strips

#define NUM_OF_MODES 1              // Number of modes

#define NUM_OF_PALETTES 1           // Number of color palettes
/*-------------*/


/* Debug settings */

/* 1 to enable output */
#define CONF_ENABLE_DEBUG   1 
#define CONF_ENABLE_INFO    1
#define CONF_ENABLE_WARN    1
#define CONF_ENABLE_ERROR   1

/*
 *  example DEBUG("something unfortunate")
 *  output to serial:
 *  DEBUG: something unfortunate
 */
#define CONF_APPEND_DEBUG_LEVEL 1


/*
 * @brief Flushes debug output after every write.
 *
 */
#define CONF_FLUSH_DEBUG 1

/* Enables color for terminal */
#define CONF_ENABLE_TERMINAL_COLOR 1

/* Select text color for debug output*/
#define CONF_DEBUG_COLOR WHITE 
#define CONF_INFO_COLOR CYAN
#define CONF_WARN_COLOR YELLOW
#define CONF_ERROR_COLOR RED 