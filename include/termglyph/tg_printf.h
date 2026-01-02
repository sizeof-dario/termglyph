/*************************************************************************//**
 * 
 * @file tg_printf.h
 * 
 * @brief Header file dedicated to the tg_printf function
 * 
 *****************************************************************************/
#ifndef TERMGLYPH_PRINTF_H
#define TERMGLYPH_PRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "text_attributes.h"



typedef struct cell
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t luma;
    char c;
} cell;


#define TG_ASCII_RAMP " .:-=+*#%@"
#define TG_ASCII_RAMP_LENGTH 10


#define TG_PRINTPPM_MODE_ASCII      1
#define TG_PRINTPPM_MODE_COLOURED   2


/**
 * @brief Writes formatted output to stdout, with support for text attributes.
 * 
 * @param format A format string that may contain both standard format
 *      specifiers (%) and extended format specifiers (#).
 * 
 *      Standard specifiers behave like in printf, except for the character '#'
 *      being reserved. To obtain the literal behaviour used by printf, each
 *      '#' must be doubled as '##'. Extended specifiers modify text colors and
 *      styles.
 * 
 *      Color specifiers:
 * 
 *      - `#df` Sets foreground color to a direct color.
 * 
 *      - `#db` Sets background color to a direct color.
 * 
 *      - `#if` Sets foreground color to an indexed color.
 * 
 *      - `#ib` Sets background color to an indexed color.
 * 
 *      - `#0f` Resets the foreground color.
 * 
 *      - `#0b` Resets the background color.
 * 
 *      - `#0c` Resets both foreground and background colors.
 * 
 *      Style specifiers:
 *  
 *      - `#n` Enables inverse mode (swaps foreground and background colors).
 * 
 *      - `#0n` Disables inverse mode.
 * 
 *      - `#o` Enables bold style.
 * 
 *      - `#0o` Disables bold style.
 * 
 *      - `#m` Enables dim style.
 * 
 *      - `#0m` Disables dim style.
 * 
 *      - `#t` Enables italic style.
 * 
 *      - `#0t` Disable italic style.
 * 
 *      - `#u` Enables underline.
 * 
 *      - `#0u` Disable underline.
 * 
 *      - `#w` Enables double underline.
 * 
 *      - `#0w` Disables double underline.
 * 
 *      - `#k` Enables blinking.
 * 
 *      - `#0k` Disables blinking. 
 * 
 *      - `#h` Hides the text.
 * 
 *      - `#0h` Disables hidden text.
 * 
 *      - `#s` Enables strikethrough.
 * 
 *      - `#0s` Disables strikethrough.
 * 
 *      Others:
 * 
 *      - `##` Writes a literal '#' character.
 * 
 *      - `#0` Resets all colors and styles.
 * 
 *      Color specifiers require additional parameters.
 * 
 * @param ... If the format string contains any standard format specifier or
 *      color specifiers, the function expects additional arguments.
 *      The parameters corrisponding to color specifiers must appear first,
 *      in the same order as their corresponding specifiers in the format
 *      string. After these, the remaining arguments correspond to the printf
 *      like format specifiers, following the same rules as printf.
 *      
 *      Direct colors can be passed as 24-bit values, either as a an (unsigned)
 *      integer or constructed from separate RGB components using the `TG_RGB`
 *      macro. Indexed colors can be passed using the `TG_INDEXED_COLOR`
 *      macros.
 * 
 * @return On success, returns the number of characters written to stdout.
 *      
 *      On failure, returns -1.
 *
 * @warning Passing arguments in an order that does not match the format string
 *          results in undefined behaviour.
 * 
 * @note The function uses ANSI escape sequences to control text colors and
 *      styles. They do not contribute to the returned character count.
 *
 */
int tg_printf(const char *format, ...);

/**
 * 
 */
int tg_printppm(const char *path, uint8_t modes);

#endif // TERMGLYPH_PRINTF_H
