#include "../include/termglyph/text_attributes.h"

void tg_to_rgbcolor_sequence(
    tg_direct_color_sequence direct_color_sequence, 
    const unsigned int rgb_value, 
    const tg_terminal_layer terminal_layer)
{
    // Prepares rgbcolor_sequence.
    strcpy(direct_color_sequence, TG_DIRECT_COLOR_SEQUENCE_TEMPLATE);
    direct_color_sequence[2] = terminal_layer;

    // Extracts value components.
    uint8_t r = TG_RGB_GET_R(rgb_value);
    uint8_t g = TG_RGB_GET_G(rgb_value);
    uint8_t b = TG_RGB_GET_B(rgb_value);

    // Converts value components into characters and completes rgbcolorseq_p.
    for (uint8_t i = 0; i < 3; i++)
    {
        // Converts first decimal digit to character, then divides by ten to
        // shift digits towards the right. 
        direct_color_sequence[9 - i] = r % 10 + '0';
        r /= 10;

        direct_color_sequence[13 - i] = g % 10 + '0';
        g /= 10;

        direct_color_sequence[17 - i] = b % 10 + '0';
        b /= 10;
    }
}