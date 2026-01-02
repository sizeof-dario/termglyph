#include "../include/termglyph/tg_printf.h"



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
                    tg_to_rgbcolor_sequence(direct_color_sequence_temp, 
                        va_arg(ap, int), TG_TERMINAL_LAYER_FOREGROUND);                    
                }
                else if(format[ftmidx + 2] == 'b') // Abs. background color.
                {
                    tg_to_rgbcolor_sequence(direct_color_sequence_temp, 
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
