#include "track.h"

#include "draw.h"
#include "time.h"

Track TRACKS[] = {
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
int NUM_TRACKS = sizeof(TRACKS)/sizeof(Track);

u_char COLORCYCLE[] = {
    255, 0, 255,
    0, 255, 255,
};

void track_setup() {
    Track *tr = TRACKS;
    int f_x = 0, f_y = 0;
    for (int i = 0; i < NUM_TRACKS; ++i) {
        tr->f_x0 = f_x;
        tr->f_y0 = f_y;
        f_x += tr->dx * ONE;
        f_y += tr->dy * ONE;
    }
}

Track* get_track(int i) {
    return &TRACKS[i];
};

void draw_tracks(short offsetx, short offsety) {
    int cn = sizeof(COLORCYCLE);
    int ci = ((get_time() / 6) * 3) % cn;
    Track *tr = TRACKS;
    short x = offsetx, y = offsety;
    ColorVertex vertices[4];
    vertices[0].b = vertices[1].b = vertices[2].b = vertices[3].b = 255;
    for (int i = 0; i < NUM_TRACKS; i += 4) {
        vertices->x = x;
        vertices->y = y;
        vertices->r = COLORCYCLE[ci];
        vertices->g = COLORCYCLE[ci+1];
        vertices->b = COLORCYCLE[ci+2];
        for (int j = 1; j < 4; ++j) {
            x += tr[i+j].dx;
            y += tr[i+j].dy;
            ci = (ci + 3) % cn;
            vertices[j].x = x;
            vertices[j].y = y;
            vertices[j].r = COLORCYCLE[ci];
            vertices[j].g = COLORCYCLE[ci+1];
            vertices[j].b = COLORCYCLE[ci+2];
        }
        draw_3lines_gouraud(vertices);
    }
}