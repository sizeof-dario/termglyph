#include "include/termglyph.h"






int main(int argc, char** argv)
{
    tg_printppm(argv[1], TG_PRINTPPM_MODE_ASCII | TG_PRINTPPM_MODE_COLOURED);
    return 0;
}
