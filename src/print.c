#include "../include/termglyph/print.h"



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
 * @note This function is private to tg_printf.
 */
static void to_rgbcolor_sequence(
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



int tg_printf(const char *format, ...)
{
    // ---------------------------------- 01 ----------------------------------
    // The function uses an internal buffer to store an intermediate format
    // string obtained by resolving all extended format specifiers and leaving
    // standard format specifiers to printf. Thus, the first body section is
    // reserved to calculating the intermediate buffer size and to its
    // allocation and initialization.
    
    // A reset-all-modes sequence will be appended at the end of the buffer, so
    // its side needs to accomodate for it and thus is initialized as
    // `TG_TEXT_STYLE_SEQUENCE_LENGTH`.
    size_t bufsize = TG_TEXT_STYLE_SEQUENCE_LENGTH; // Internal buffer size.

    for (size_t i = 0; format[i]; i++)
    {
        if (format[i] == '#')
        {
            // By doing this, every possible extended specifier, even the
            // invalid ones, are treated as valid absolute color sequences
            // specifiers. This means the buffer will almost surely be larger
            // than necessary; however, I chose code simplicty over strict size
            // optimization. 
            bufsize += TG_DIRECT_COLOR_SEQUENCE_LENGTH;
        }
        else // format[i] != '#'
        {
            bufsize++;
        }      
    }

    char *buffer = (char*)malloc(bufsize);
    if(!buffer)
    {
        return -1;
    }
    memset(buffer, 0, bufsize); // TBh, just as extra safety meausre.

    // ---------------------------------- 02 ----------------------------------
    // This body section is dedicated to resolving extended format specifiers.

    int bufidx = 0; // Index iterating over the buffer.
    int ftmidx = 0; // Index iterating over the format string.

    int written = 0; // The function return value.

    // A temporary `rgbocolor_sequence` used for intermediate manipulations.
    tg_direct_color_sequence direct_color_sequence_temp;

    va_list ap; // Arguement pointer for variadic arguments.

    va_start(ap, format);
    while(bufidx < bufsize && format[ftmidx])
    {
        if (format[ftmidx] == '#')
        {
            switch (format[ftmidx + 1])
            {
            case 'd': // Absolute color sequences.
                // First part consists in converting the RGB 24-bit value to a
                // proper ANSI escape sequence.
                if(format[ftmidx + 2] == 'f') // Absolute foreground color.
                {
                    to_rgbcolor_sequence(direct_color_sequence_temp, 
                        va_arg(ap, int), TG_TERMINAL_LAYER_FOREGROUND);                    
                }
                else if(format[ftmidx + 2] == 'b') // Abs. background color.
                {
                    to_rgbcolor_sequence(direct_color_sequence_temp, 
                        va_arg(ap, int), TG_TERMINAL_LAYER_BACKGROUND);
                }
                else // Invalid specifier
                {   
                    buffer[bufidx++] = format[ftmidx++];
                    // We ignore the rest of this case-code if the specifier is
                    // invalid.
                    break; 
                }

                // If the specifier is valid, and so we got a rgbcolor
                // sequence, we now need to insert it into the buffer.
                strcpy(buffer + bufidx, direct_color_sequence_temp);
                // Excluding the null-terminator in the count takes a -1.
                bufidx += TG_DIRECT_COLOR_SEQUENCE_LENGTH - 1;
                ftmidx += 3; // The specifier length.
                // Since we do not want ANSI sequences to count towards the
                // number of written characters, we subtract such sequences
                // length to `written` every time we insert one of them into
                // the buffer.
                written -= TG_DIRECT_COLOR_SEQUENCE_LENGTH - 1;
                break;

            case 'i': // Relative color sequences.
                if(format[ftmidx + 2] == 'f') // Relative foreground color.
                {
                    strcpy(buffer + bufidx, va_arg(ap, char*));                  
                }
                else if(format[ftmidx + 2] == 'b') // Rel. Background color.
                {
                    strcpy(buffer + bufidx, va_arg(ap, char*));
                    // The macros are defined to be the foreground sequcndes by
                    // default, so they need to be modified for background
                    // color changes. The following if-else block check whether
                    // the color is bright or not.
                    if(*(buffer + bufidx + 3) == '3') // non-bright version
                    {
                        *(buffer + bufidx + 3) = '4';
                    }
                    else // it's = to '9' -> bright version
                    {
                        *(buffer + bufidx + 2) = '1';
                        *(buffer + bufidx + 3) = '0';
                    }
                    
                }
                else // Invalid specifier.
                {  
                    buffer[bufidx++] = format[ftmidx++];
                    break;
                }

                bufidx += TG_INDEXED_COLOR_SEQUENCE_LENGTH - 1;
                ftmidx += 3;
                written -= TG_INDEXED_COLOR_SEQUENCE_LENGTH - 1;
                break;

            case 'o': // bold style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_BOLD);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 'm': // dim style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_DIM);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 't': // italic style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_ITALIC);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 'u': // underline style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_UNDERLINE);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 'k': // blinking style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_BLINKING);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 'n': // inverse style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_INVERSE);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 'w': // double underline style
            strcpy(buffer + bufidx, TG_TEXT_STYLE_DOUBLE_UNDERLINE);   
            bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
            ftmidx += 2;
            written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
            break;

            case 'h': // hidden style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_HIDDEN);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case 's': // strikethrough style
                strcpy(buffer + bufidx, TG_TEXT_STYLE_STRIKETHROUGH);   
                bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                ftmidx += 2;
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;

            case '0': // reset modes
                switch (format[ftmidx + 2]) // check which specific mode it is
                {
                case 'o': // reset bold style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_BOLD_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;
                
                case 'm': // reset dim style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_DIM_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 't': // reset italic style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_ITALIC_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'u': // reset underline style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_UNDERLINE_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'k': // reset blinking style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_BLINKING_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;
                
                case 'f': // reset foreground color
                    strcpy(buffer + bufidx, TG_RESET_FOREGROUND_COLOR);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'b': // reset background color
                    strcpy(buffer + bufidx, TG_RESET_BACKGROUND_COLOR);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'c': // reset all colors
                    strcpy(buffer + bufidx, TG_RESET_FOREGROUND_COLOR);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    strcpy(buffer + bufidx, TG_RESET_BACKGROUND_COLOR);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'n': // reset inverse style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_INVERSE_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'w': // reset double underline style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_DOUBLE_UNDERLINE_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 'h': // reset hidden style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_HIDDEN_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                case 's': // reset strikethrough style
                    strcpy(buffer + bufidx, TG_TEXT_STYLE_STRIKETHROUGH_RESET);   
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 3;
                    break;

                default:
                    // If there is nothing extra, the reset-all-modes sequence
                    // is considered.

                    strcpy(buffer + bufidx, TG_RESET_ALL_MODES);
                    bufidx += TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                    ftmidx += 2;
                    break;
                }
                written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;
                break;
                
            case '#': // case for ##
                buffer[bufidx++] = '#';
                ftmidx += 2;
                break;

            default:
                buffer[bufidx++] = format[ftmidx++];
                break;
            }
            
        }
        else // format[ftmidx] != '#'
        {
            // It's just a regular character (or standard specifier part) so we
            // copy it as it is.
            buffer[bufidx++] = format[ftmidx++];
        }
    }

    // Adding final reset-all-modes sequence
    strcpy(buffer + bufidx, TG_RESET_ALL_MODES);
    written -= TG_TEXT_STYLE_SEQUENCE_LENGTH - 1;

    // ---------------------------------- 03 ----------------------------------
    // Once the buffer string is left with only the standard specifiers, it is
    // passed to vprintf.

    // Calls vprintf and updates written
    written += vprintf(buffer, ap);

    // Cleans and return
    va_end(ap);
    free(buffer);
    return written;
}



typedef struct tg_rgb_char
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    char c;
} tg_rgb_char;



#define TG_ASCII_RAMP " .:-=+*#%@"
#define TG_ASCII_RAMP_LENGTH 10



/**
 * @brief Reads a token from a ppm image file.
 * 
 * @param f The file opened in bynary mode.
 * 
 * @param token_out A pointer to int to store the reed token in.
 * 
 * @return 0 on success, a non-zero value on failure.
 * 
 * @note This function is private to tg_printppm.
 */
static int ppm_read_token(FILE *f, int *token_out)
{
    int c = 0;
    while (1) // Looping up to a token
    {
        // Get a character
        c = fgetc(f); 
        
        // Check if it's EOF
        if(c == EOF)
        {
            return 1;
        }
        
        // Check if it's a whitespace
        if(isspace((uint8_t)c))
        {
            // If so there's nothing more to do with this character
            continue;
        }

        // Check if it marks the beginning of a '#'-comment
        if(c == '#')
        {
            // If so, we skip across the whole comment
            do
            {
                c = fgetc(f);
                // We also need to make sure we don't pick up an EOF
                if(c == EOF)
                {
                    return 1;
                }
            } while (c != '\n');    
        }

        // If we neither pick up an EOF, a whitespace or a comment, it means we
        // have found a token
        break;
    }

    // We put back the last read character, still checking if `ungetc` returns
    // EOF because it is not guaranteed to succeed
    if(ungetc(c, f) == EOF)
    {
        return 1;
    }

    // We can now properly read a token, while making sure that fscanf does not
    // fail in any case
    if(fscanf(f, "%d", token_out) != 1)
    {
        return 1;
    }

    // Return 0 if succeded
    return 0;
}



int tg_printppm(const char* path)
{
    // ---------------------------------- 01 ---------------------------------- 
    // FIle opening.
    FILE *f = fopen(path, "rb");
    if (!f)
    {
        return 1;
    }

    // ---------------------------------- 02 ---------------------------------- 
    // Reading the file header and possibily reading the pixel data.

    // First thing to check is the magic number (P6).
    char magic_number_left = fgetc(f);
    char magic_number_right = fgetc(f);
    if (magic_number_left != 'P' || magic_number_right != '6')
    {
        fclose(f);
        return 1; 
    }

    // Now we can get width, height, maxval and check that maxval is 255.
    int width = 0;
    int height = 0;
    int maxval = 0;
    if (ppm_read_token(f, &width)  || ppm_read_token(f, &height) || 
        ppm_read_token(f, &maxval) || maxval != 255)
    {
        fclose(f);
        return 1;
    }

    // After maxval, an unspecified number of whitespaces can ve present before
    // the pixel data, so we need to read them out.
    int c;
    do {
        c = fgetc(f);
        if (c == EOF)
        {
            fclose(f);
            return 1;
        }
    } while (isspace((uint8_t)c));
    // We put back the last byte we got since it must be a pixel data byte for
    // us to be out of the loop.
    ungetc(c, f);

    // If everything went fine, we can now proceed to the pixel data reading.
    uint8_t *pixels = (uint8_t*)malloc(3 * width * height);
    if (!pixels)
    {
        fclose(f);
        return 1;
    }
    fread(pixels, 3, width * height, f);
    // We do not need to keep `f` open anymore.
    fclose(f);

    // ---------------------------------- 03 ----------------------------------
    // We now proceed to convert the pixel data to...
    tg_rgb_char *buffer = (tg_rgb_char*)malloc(
        (width * height + height + 1) * sizeof(tg_rgb_char));

    int j = 0;
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int i = y * width + x;
            buffer[j].r = pixels[3*i];
            buffer[j].g = pixels[3*i+1];
            buffer[j].b = pixels[3*i+2];
            uint8_t luma = 0.2126*buffer[j].r 
                         + 0.7152*buffer[j].g 
                         + 0.0722*buffer[j].b;
            buffer[j].c = TG_ASCII_RAMP[luma * (TG_ASCII_RAMP_LENGTH - 1)/255];
            j++;
        }
        buffer[j].r = 0;
        buffer[j].g = 0;
        buffer[j].b = 0;
        buffer[j].c = '\n';
        j++;
    }
    buffer[j].r = 0;
    buffer[j].g = 0;
    buffer[j].b = 0;
    buffer[j].c = '\0';

    for (size_t i = 0; i < width * height + height + 1; i++)
    {
        tg_printf("#db%c",
            TG_RGB(buffer[i].r, buffer[i].g, buffer[i].b),
            buffer[i].c);
    }

    free(buffer);
    free(pixels);
    return 0;
}


