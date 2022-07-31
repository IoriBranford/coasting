#include "ground.h"

#include "draw.h"

typedef struct Ground {
    short dx, dy;
} Ground;

Ground GROUND[] = {
    {16, 0},
    {16, 0},
    {16, 0},
    {16, 0},

    {15, 1},
    {14, 2},
    {13, 3},
    {12, 4},

    {11, 5},
    {10, 6},
    {9, 7},
    {8, 8},

    {8, 8},
    {8, 8},
    {8, 8},
    {8, 8},

    {8, 8},
    {9, 7},
    {10, 6},
    {11, 5},

    {12, 4},
    {13, 3},
    {14, 2},
    {15, 1},

    {16, 0},
    {16, 0},
    {16, 0},
    {16, 0},
};

void draw_ground() {
    int n = sizeof(GROUND)/sizeof(Ground);
    Ground *g = GROUND;
    short x = 0, y = 0;
    ColorVertex vertices[4];
    vertices[0].b = vertices[1].b = vertices[2].b = vertices[3].b = 255;
    for (int i = 0; i < n; i += 4) {
        vertices->x = x;
        vertices->y = y;
        for (int j = 1; j < 4; ++j) {
            x += g[i+j].dx;
            y += g[i+j].dy;
            vertices[j].x = x;
            vertices[j].y = y;
        }
        if ((i/4) % 2){
            vertices[0].r = vertices[1].g = vertices[2].r = vertices[3].g = 0;
            vertices[0].g = vertices[1].r = vertices[2].g = vertices[3].r = 255;
        } else {
            vertices[0].r = vertices[1].g = vertices[2].r = vertices[3].g = 255;
            vertices[0].g = vertices[1].r = vertices[2].g = vertices[3].r = 0;
        }
        draw_3lines_gouraud(vertices);
    }
}