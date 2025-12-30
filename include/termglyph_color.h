/************************************************************************ *//*
 * 
 * @file termglyph_color.h
 * 
 * @brief Contains color related type, macro and function definitions.
 * 
 *****************************************************************************/
#ifndef TERMGLYPH_COLOR_H
#define TERMGLYPH_COLOR_H



#include <stdint.h>
#include <string.h>



/****************************** Absolute colors ******************************/



/**
 * @brief Size of a `tg_rgbcolor_sequence` storing a 24-bit color escape sequence.
 * 
 * A value of 20 is chosen to accommodate for the maximum length such sequences
 * can reach, including the null-terminator.
 * 
 */
#define TG_RGBCOLOR_SEQUENCE_LENGTH 20



/**
 * @brief Represents an escape sequence used to change colors in terminals that
 *      support 24-bit colors.
 * 
 * Such sequences take the form "E[L8;2;R;G;Bm", where
 * 
 *      - E is the escape character.
 * 
 *      - L is either 3 (for foreground) or 4 (for background).
 * 
 *      - R, G and B are respectively the red, green and blue components of the
 *        represented color.
 * 
 * The sequence is stored as a null-terminated string of length 
 * `TG_RGBCOLOR_SEQUENCE_LENGTH`.
 * 
 */
typedef char tg_rgbcolor_sequence[TG_RGBCOLOR_SEQUENCE_LENGTH];



/** 
 * @brief Template string used by the `tg_torgbcolorseq` function to build a
 *      color sequence based on the provided arguments.
 */
#define TG_RGBCOLOR_SEQUENCE_TEMPLATE "\033[08;2;000;000;000m"



/**
 * @brief Specifies the terminal layer for functions using `tg_rgbcolor_sequence`.
 *
 * This type is used to indicate whether a color should be applied to the
 * foreground or the background.
 * 
 */
typedef enum tg_termlayer { 
    TG_TERMLAYER_FOREGROUND = '3', /**< Foreground layer. */
    TG_TERMLAYER_BACKGROUND = '4'  /**< Background layer. */
} tg_termlayer;



/**
 * @brief Converts a 24-bit RGB color value into a `tg_rgbcoloreq` type.
 * 
 * @param rgbcolorseq A `tg_rgbcolor_sequence` to store the conversion result.
 * @param value The 24-bit RGB value of the color to convert.
 * @param layer The terminal layer of the color to convert, either being
 *      `FOREGROUND` or `BACKGROUND`.
 * 
 */
void tg_to_rgbcolor_sequence(
    tg_rgbcolor_sequence rgbcolor_sequence, 
    const int rgb_value, 
    const tg_termlayer layer
);



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



/****************************** Relative colors ******************************/



/**
 * @brief Size of a `tg_colorseq` macro storing a color escape sequence.
 * 
 * A value of 7 is chosen to accommodate for the maximum length such sequences
 * can reach, including the null-terminator.
 * 
 */
#define TG_COLORSEQ_LENGTH 7



// tg_colorseq macros for relative colors

#define TG_COLOR_BLACK          "\033[030m"
#define TG_COLOR_RED            "\033[031m"
#define TG_COLOR_GREEN          "\033[032m"
#define TG_COLOR_YELLOW         "\033[033m"
#define TG_COLOR_BLUE           "\033[034m"
#define TG_COLOR_MAGENTA        "\033[035m"
#define TG_COLOR_CYAN           "\033[036m"
#define TG_COLOR_WHITE          "\033[037m"

#define TG_COLOR_BRIGHT_BLACK   "\033[090m"
#define TG_COLOR_BRIGHT_RED     "\033[091m"
#define TG_COLOR_BRIGHT_GREEN   "\033[092m"
#define TG_COLOR_BRIGHT_YELLOW  "\033[093m"
#define TG_COLOR_BRIGHT_BLUE    "\033[094m"
#define TG_COLOR_BRIGHT_MAGENTA "\033[095m"
#define TG_COLOR_BRIGHT_CYAN    "\033[096m"
#define TG_COLOR_BRIGHT_WHITE   "\033[097m"



#endif // TERMGLYPH_COLOR_H
