#include "background.h"

#include "draw.h"

ColorVertex sky[4] = {
    {.x = 0, .y =   120,    .r = 0x4c,  .g = 0x05, .b = 0x68 },
    {.x = 320, .y = 120,    .r = 0x4c,  .g = 0x05, .b = 0x68 },
    {.x = 0, .y =   0,      .r = 0x1e,  .g = 0x09, .b = 0x45 },
    {.x = 320, .y = 0,      .r = 0x1e,  .g = 0x09, .b = 0x45 },
};
ColorVertex ground[4] = {
    {.x = 0, .y =   240, .r = 0x24, .g = 0x0b, .b = 0x41 },
    {.x = 320, .y = 240, .r = 0x24, .g = 0x0b, .b = 0x41 },
    {.x = 0, .y =   120, .r = 0x31, .g = 0x1c, .b = 0x68 },
    {.x = 320, .y = 120, .r = 0x31, .g = 0x1c, .b = 0x68 },
};

void draw_background(short camerax, short cameray) {
    ColorVertex groundline[2] = {
        {.x = 0, .y = 120, .r = 0x5f, .g = 0x0e, .b = 0x87 },
        {.x = 0, .y = 240, .r = 0x5f, .g = 0x0e, .b = 0x87 },
    };
    int linex = ((camerax * ONE / 16 / ONE) % 80) - 400;
    for (int i = 0; i < 16; ++i) {
        ColorVertex *v0 = groundline;
        ColorVertex *v1 = groundline+1;
        v1->x = linex;
        v0->x = 160 + (linex-160)*ONE/16/ONE;
        linex += 80;
        draw_line_flat(groundline);
    }
    draw_quad_flat(ground);
    draw_quad_gouraud(sky);
}