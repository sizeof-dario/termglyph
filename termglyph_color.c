#include "include/termglyph_color.h"

void tg_to_rgbcolor_sequence(
    tg_rgbcolor_sequence rgbcolor_sequence, 
    const int rgb_value, 
    const tg_termlayer layer)  
{
    // Prepares rgbcolor_sequence.
    strcpy(rgbcolor_sequence, TG_RGBCOLOR_SEQUENCE_TEMPLATE);
    rgbcolor_sequence[2] = layer;

    // Extracts value components.
    uint8_t r = TG_RGB_GET_R(rgb_value);
    uint8_t g = TG_RGB_GET_G(rgb_value);
    uint8_t b = TG_RGB_GET_B(rgb_value);

    // Converts value components into characters and completes rgbcolorseq_p.
    for (uint8_t i = 0; i < 3; i++)
    {
        // Converts first decimal digit to character, then divides by ten to
        // shift digits towards the right. 
        rgbcolor_sequence[9 - i] = r % 10 + '0';
        r /= 10;

        rgbcolor_sequence[13 - i] = g % 10 + '0';
        g /= 10;

        rgbcolor_sequence[17 - i] = b % 10 + '0';
        b /= 10;
    }
}