/*********************************************************************** *//**
 * 
 * @file termglyph_printf.h
 * 
 * @brief Header file dedicated to the tg_printf function
 * 
 *****************************************************************************/
#ifndef TERMGLYPH_PRINTF_H
#define TERMGLYPH_PRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "termglyph_color.h"

#define TG_TEXT_STYLE_SEQUENCE_LENGTH 6

#define TG_RESET_ALL_MODES                      "\033[00m"

#define TG_RESET_FOREGROUND_COLOR               "\033[39m"
#define TG_RESET_BACKGROUND_COLOR               "\033[49m"

#define TG_TEXT_STYLE_BOLD                      "\033[01m"
#define TG_TEXT_STYLE_DIM                       "\033[02m"
#define TG_TEXT_STYLE_ITALIC                    "\033[03m"
#define TG_TEXT_STYLE_UNDERLINE                 "\033[04m"
#define TG_TEXT_STYLE_BLINKING                  "\033[05m"
#define TG_TEXT_STYLE_INVERSE                   "\033[07m"
#define TG_TEXT_STYLE_HIDDEN                    "\033[08m"
#define TG_TEXT_STYLE_STRIKETHROUGH             "\033[09m"
#define TG_TEXT_STYLE_DOUBLE_UNDERLINE          "\033[21m"

#define TG_TEXT_STYLE_BOLD_RESET                "\033[22m"
#define TG_TEXT_STYLE_DIM_RESET                 "\033[22m"
#define TG_TEXT_STYLE_ITALIC_RESET              "\033[23m"
#define TG_TEXT_STYLE_UNDERLINE_RESET           "\033[24m"
#define TG_TEXT_STYLE_BLINKING_RESET            "\033[25m"
#define TG_TEXT_STYLE_INVERSE_RESET             "\033[27m"
#define TG_TEXT_STYLE_HIDDEN_RESET              "\033[28m"
#define TG_TEXT_STYLE_STRIKETHROUGH_RESET       "\033[29m"
#define TG_TEXT_STYLE_DOUBLE_UNDERLINE_RESET    "\033[24m"

/**
 * @brief Writes formatted output to stdout.
 * 
 * @param format A format string that may contain both standard format
 *      specifiers (%) as long as extended format specifiers (#).
 * 
 *      Standard specifiers behave like in printf, except for the character '#'
 *      being reserved. To obtain the literal behaviour used by printf, each
 *      '#' must be doubled as '##'.
 * 
 *      Extended specifiers control changes in text colors and attributes:
 * 
 *      - `#af` Sets foreground color to an absolute color.
 * 
 *      - `#ab` Sets background color to an absolute color.
 * 
 *      - `#rf` Sets foreground color to a relative color.
 * 
 *      - `#rb` Sets background color to a relative color.
 * 
 *      - `#0f` Resets foreground color.
 * 
 *      - `#0b` Resets background color.
 * 
 *      - `#0c` Resets all colors.
 *  
 *      - `#n' Inverse mode.
 * 
 *      - `#0n` Disable reverse mode.
 * 
 *      - `#B` Makes the text bold.
 * 
 *      - `#0B` Resets bold attribute.
 * 
 *      - `#d` Makes the text dim.
 * 
 *      - `#0d` Resets dim attribute.
 * 
 *      - `#i` Makes the text italic.
 * 
 *      - `#0i` Resets italic attribute.
 * 
 *      - `#u` Underlined text.
 * 
 *      - `#0u` Resets underlined attribute.
 * 
 *      - `#U` Double Underlined text.
 * 
 *      - `#0U` Double Resets underlined attribute.
 * 
 *      - `#l` Blinking text.
 * 
 *      - `#0l` Resets blinking attribute. 
 * 
 *      - `#h` Hidden mode.
 * 
 *      - `#0h` Disable hidde mode.
 * 
 *      - `#s` strikethrough mode. TO DO
 * 
 *      - `#0s` disable strikethrough mode. TO DO
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
 *      in the same order as their corresponding specifiers in the
 *      format string. After these, the remaining arguments correspond to the
 *      printf-like format specifiers, following the same rules as printf.
 * 
 * @return If the function succeeds, it returns the number of characters
 *      written to stdout. The function uses ANSI escape sequences to alter
 *      text colors and attributes; these do not count to the return value.
 *      
 *      If the function fails, returns -1.
 *
 * @warning Passing arguments in the wrong order results in undefined
 *      behaviour.
 * 
 * @note For more about absolute and relative colors, check
 *
 */
int tg_printf(const char *format, ...);

#endif // TERMGLYPH_PRINTF_H
