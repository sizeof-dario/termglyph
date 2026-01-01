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

// vvv still needs to be properly written

/**
 * @brief Writes formatted output to stdout.
 * 
 * @param format A format string that may contain both standard format
 *      specifiers (%) and extended format specifiers (#).
 * 
 *      Standard specifiers behave like in printf, except for the character '#'
 *      being reserved. To obtain the literal behaviour used by printf, each
 *      '#' must be doubled as '##'.
 * 
 *      Extended specifiers control changes in text attributes:
 * 
 *      - `#df` Sets foreground color to a direct color passed as additional
 *         parameter.
 * 
 *      - `#db` Sets background color to a direct color passed as additional
 *         parameter.
 * 
 *      - `#if` Sets foreground color to an indexed color passed as additional
 *         parameter.
 * 
 *      - `#ib` Sets background color to an indexed color passed as additional
 *         parameter.
 * 
 *      - `#0f` Resets the foreground color.
 * 
 *      - `#0b` Resets the background color.
 * 
 *      - `#0c` Resets all colors.
 *  
 *      - `#n` Inverse mode (switches foreground and background colors).
 * 
 *      - `#0n` Disables reverse mode.
 * 
 *      - `#o` Makes the text bold.
 * 
 *      - `#0o` Resets the bold style.
 * 
 *      - `#d` Makes the text dim.
 * 
 *      - `#0d` Resets the dim style.
 * 
 *      - `#i` Makes the text italic.
 * 
 *      - `#0i` Resets the italic style.
 * 
 *      - `#u` Underlines text.
 * 
 *      - `#0u` Resets the underline style.
 * 
 *      - `#w` Adds ad double underline to the text.
 * 
 *      - `#0w` Resets the double underline style.
 * 
 *      - `#k` Makes the text blinking.
 * 
 *      - `#0k` Resets the blinking style. 
 * 
 *      - `#h` Hides text.
 * 
 *      - `#0h` Resets the hidden style.
 * 
 *      - `#s` Applies the strikethrough style.
 * 
 *      - `#0s` Resets the strikethrough style.
 * 
 *      - `##` Writes a literal '#' character.
 * 
 *      - `#0` Resets all colors and attributes.
 * 
 * Extended specifiers that set colors requires the function to receive
 * additional parameters.
 * 
 * @param ... If the format string contains any standard format specifier or
 *      color setting specifiers, the function expects additional arguments.
 *      The parameters corrisponding to extended specifiers must appear first,
 *      in the same order as their corresponding specifiers in the format
 *      string. After these, the remaining arguments correspond to the printf
 *      like format specifiers, following the same rules as printf.
 *      
 *      Color related format specifiers can be 24-bit colors (for direct
 *      colors) and can be both passed as intigers or as separate rgb
 *      components with the help of the `TG_RGB` macro, or they can be indexed
 *      colors, for which the TG_INDEXED_COLOR macros exist.
 * 
 * @return If the function succeeds, it returns the number of characters
 *      written to stdout.
 *      
 *      If the function fails, it returns -1.
 *
 * @warning Passing arguments in the wrong order results in undefined
 *      behaviour.
 * 
 * @note The function uses ANSI escape sequences to alter text colors and
 *      styles; these do not add to the return value.
 *
 */
int tg_printf(const char *format, ...);

#endif // TERMGLYPH_PRINTF_H
