#include "track.h"

#include "draw.h"
#include "time.h"

#include <abs.h>

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

    {15, -1},
    {14, -2},
    {13, -3},
    {12, -4},

    {11, -5},
    {10, -6},
    {9,  -7},
    {8,  -8},

    {8, -8},
    {8, -8},
    {8, -8},
    {8, -8},

    {8,  -8},
    {9,  -7},
    {10, -6},
    {11, -5},

    {12, -4},
    {13, -3},
    {14, -2},
    {15, -1},

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
    int x = 0, y = 0;
    for (int i = 0; i < NUM_TRACKS; ++i) {
        tr->x0 = x;
        tr->y0 = y;
        tr->f_x0 = ONE * x;
        tr->f_y0 = ONE * y;

        int f_dx = tr->dx * ONE;
        int f_dy = tr->dy * ONE;
        int f_angle = ratan2(f_dy, f_dx);
        tr->f_angle = f_angle;
        tr->f_dirx = ccos(f_angle);
        tr->f_diry = csin(f_angle);
        x += tr->dx;
        y += tr->dy;

        tr++;
    }
}

void move_on_tracks(int *f_x, int *f_y, int *f_pos, int *tri, int *f_angle, int f_speed) {
    // int f_speed = ONE;
    int f_newx = *f_x, f_newy = *f_y;
    int f_newpos = *f_pos;
    int newtri = *tri;

    int f_move = f_speed;

    while (newtri >= 0 && newtri < NUM_TRACKS && f_move) {
        Track *tr = &TRACKS[newtri];
        *f_angle = tr->f_angle;
        int f_destx = tr->f_x0;
        int f_desty = tr->f_y0;
        if (f_move > 0) {
            f_destx += ONE * tr->dx;
            f_desty += ONE * tr->dy;
        }

        int distx = (f_destx - f_newx) / ONE;
        int disty = (f_desty - f_newy) / ONE;
        int distsq = (distx * distx) + (disty * disty);
        int f_len = csqrt(distsq);
        int f_absmove = abs(f_move);
        if (f_absmove >= f_len) {
            f_newx = f_destx;
            f_newy = f_desty;
            int dir = f_move >= 0 ? 1 : -1;
            f_move -= dir*f_len;
            f_newpos += dir*f_len;
            newtri += dir;
        } else {
            f_newx += f_move * tr->f_dirx / ONE;
            f_newy += f_move * tr->f_diry / ONE;
            f_newpos += f_move;
            f_move = 0;
        }
    }
    *f_x = f_newx;
    *f_y = f_newy;
    *f_pos = f_newpos;
    *tri = newtri;
}

void draw_tracks(short offsetx, short offsety) {
    int cn = sizeof(COLORCYCLE);
    int ci = ((get_time() / 6) * 3) % cn;
    Track *tr = TRACKS;
    ColorVertex vertices[2];
    for (int i = 0; i < NUM_TRACKS; ++i) {
        vertices->x = tr->x0 + offsetx;
        vertices->y = tr->y0 + offsety;
        vertices[1].x = vertices->x + tr->dx;
        vertices[1].y = vertices->y + tr->dy;

        vertices->r = COLORCYCLE[ci];
        vertices->g = COLORCYCLE[ci+1];
        vertices->b = COLORCYCLE[ci+2];
        ci = (ci + 3) % cn;
        vertices[1].r = COLORCYCLE[ci];
        vertices[1].g = COLORCYCLE[ci+1];
        vertices[1].b = COLORCYCLE[ci+2];

        draw_line_gouraud(vertices);
        ++tr;
    }
}