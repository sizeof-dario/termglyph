/*************************************************************************//**
 * 
 * @file text_attributes.h
 * 
 * @brief Definitions of the types, macros and functions for text attributes
 *      (colors and styles).
 *
 *****************************************************************************/
#ifndef TERMGLYPH_TEXT_ATTRIBUTES_H
#define TERMGLYPH_TEXT_ATTRIBUTES_H

#include <stdint.h>
#include <string.h>



/**
 * @brief Length of a `tg_direct_color_sequence` storing a 24-bit color escape
 *      sequence.
 * 
 * A value of 20 is chosen to accommodate for the maximum length such sequences
 * can reach, including the null-terminator.
 * 
 */
#define TG_DIRECT_COLOR_SEQUENCE_LENGTH 20



/**
 * @brief Represents an escape sequence used to change colors in terminals that
 *      support 24-bit colors.
 * 
 * Such sequences take the form of "E[L8;2;RRR;GGG;BBBm", where
 * 
 *      - E is the escape character.
 * 
 *      - L is either 3 (for foreground) or 4 (for background).
 * 
 *      - RRR, GGG and BBB are respectively the red, green and blue components
 *        of the represented color.
 * 
 * The sequence is stored as a null-terminated string of length 
 * `TG_DIRECT_COLOR_SEQUENCE_LENGTH`.
 * 
 */
typedef char tg_direct_color_sequence[TG_DIRECT_COLOR_SEQUENCE_LENGTH];



/**
 * @brief Template string used by the `tg_to_direct_color_sequence` function to
 *      build a color sequence based on the provided arguments.
 * 
 */  
#define TG_DIRECT_COLOR_SEQUENCE_TEMPLATE "\033[08;2;000;000;000m"



/**
 * @brief Specifies the terminal layer for functions using
 *      `tg_direct_color_sequence`.
 *
 * This type is used to indicate whether a color should be applied to the
 * foreground or the background.
 * 
 */
typedef enum tg_terminal_layer { 
    TG_TERMINAL_LAYER_FOREGROUND = '3', /**< Foreground layer. */
    TG_TERMINAL_LAYER_BACKGROUND = '4'  /**< Background layer. */
} tg_terminal_layer;



/**
 * @brief Converts red, green and blue components to a 24-bit RGB color value.
 *  
 * @param red Red component (0-255).
 * @param green Green component (0-255).
 * @param blue Blue component (0-255).
 * 
 * @return The color of RGB components r, g and b as a 24-bit unsigned int.
 * 
 */
#define TG_RGB(r, g, b)                         \
    (uint32_t)(((uint32_t)(uint8_t)(r) << 16) | \
               ((uint32_t)(uint8_t)(g) << 8)  | \
                (uint32_t)(uint8_t)(b))



/** Extracts the red component from a 24-bit RGB color value. */
#define TG_RGB_GET_R(x) (uint8_t)((x) >> 16)



/** Extracts the green component from a 24-bit RGB color value. */
#define TG_RGB_GET_G(x) (uint8_t)((x) >> 8)



/** Extracts the blue component from a 24-bit RGB color value. */
#define TG_RGB_GET_B(x) (uint8_t)(x)



/**
 * @brief Length of a `tg_indexed_color` macro storing a color escape sequence.
 * 
 * A value of 7 is chosen to accommodate for the maximum length such sequences
 * can reach, including the null-terminator.
 * 
 */
#define TG_INDEXED_COLOR_SEQUENCE_LENGTH 7



/**
 * @name Terminal indexed colors escape sequences.
 *
 * @brief These macros are, for the user concern, intended as terminal layer
 *      independent. However, they are, by default, defined to be the
 *      foreground version of each non-bright/bright color. It's up to the
 *      function using them – like `tg_printf, with the correct specifiers – to
 *      decide whether the color get to apply to the foreground layer or the
 *      background one.
 * 
 * @{
 */
#define TG_INDEXED_COLOR_BLACK                  "\033[030m"
#define TG_INDEXED_COLOR_RED                    "\033[031m"
#define TG_INDEXED_COLOR_GREEN                  "\033[032m"
#define TG_INDEXED_COLOR_YELLOW                 "\033[033m"
#define TG_INDEXED_COLOR_BLUE                   "\033[034m"
#define TG_INDEXED_COLOR_MAGENTA                "\033[035m"
#define TG_INDEXED_COLOR_CYAN                   "\033[036m"
#define TG_INDEXED_COLOR_WHITE                  "\033[037m"

#define TG_INDEXED_COLOR_BRIGHT_BLACK           "\033[090m"
#define TG_INDEXED_COLOR_BRIGHT_RED             "\033[091m"
#define TG_INDEXED_COLOR_BRIGHT_GREEN           "\033[092m"
#define TG_INDEXED_COLOR_BRIGHT_YELLOW          "\033[093m"
#define TG_INDEXED_COLOR_BRIGHT_BLUE            "\033[094m"
#define TG_INDEXED_COLOR_BRIGHT_MAGENTA         "\033[095m"
#define TG_INDEXED_COLOR_BRIGHT_CYAN            "\033[096m"
#define TG_INDEXED_COLOR_BRIGHT_WHITE           "\033[097m"
/** @} */



/**
 * @brief Length of tg_text_stile and tg_reset macros.
 * 
 * A value of 6 is chosen to accommodate for the maximum length such sequences
 * can reach, including the null-terminator.
 * 
 */
#define TG_TEXT_STYLE_SEQUENCE_LENGTH 6



/**
 * @name General text attributes macros.
 *
 * @brief These macros include the reset escape sequences along with the
 *      sequences used to change various text styles.
 *
 * @note These macros are intended for internal use.
 *
 * @{
 */
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
/** @} */



#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Converts a 24-bit RGB color value into a `tg_direct_color_sequence`
 *      type.
 * 
 * @param direct_color_sequence A `tg_direct_color_sequence` to store the
 *      conversion result.
 * @param rgb_value The 24-bit RGB value of the color to convert.
 * @param terminal_layer The terminal layer of the color to convert, either
 *      being `TG_TERMINAL_LAYER_FOREGROUND` or `TG_TERMINAL_LAYER_BACKGROUND`.
 * 
 */
void tg_to_rgbcolor_sequence(
    tg_direct_color_sequence direct_color_sequence, 
    const unsigned int rgb_value, 
    const tg_terminal_layer terminal_layer
);



#ifdef __cplusplus
}
#endif



#endif // TERMGLYPH_TEXT_ATTRIBUTES_H
