#include "include/termglyph.h"

int main()
{
    tg_printf("#db#if             \n             \n  termglyph  \n  #t#u#o#dfh#dfi#dfg#dfh#dfl#dfi#dfg#dfh#dft#0u  \n             \n             \n",
    TG_RGB(0, 0, 0),
    TG_INDEXED_COLOR_BRIGHT_WHITE,
    TG_RGB(255,   0,   0),
    TG_RGB(255, 143,   0),
    TG_RGB(255, 255,   0),
    TG_RGB( 96, 255,   0),
    TG_RGB(  0, 255, 128),
    TG_RGB(  0, 255, 255),
    TG_RGB(  0, 128, 255),
    TG_RGB(  0,   0, 255),
    TG_RGB(128,   0, 255)
    );
}
