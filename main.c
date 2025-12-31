#include "include/termglyph.h"

int main()
{
    int w = tg_printf("Blinking #l#afR#afG#afB\n", 
        TG_RGB(255, 0, 0), 
        TG_RGB(0, 255, 0), 
        TG_RGB(0, 0, 255));

    tg_printf("Expected: 13. w = %d.\n", w);
}
