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
    int f_x = 0, f_y = 0;
    for (int i = 0; i < NUM_TRACKS; ++i) {
        tr->f_x0 = f_x;
        tr->f_y0 = f_y;

        int f_dx = tr->dx * ONE;
        int f_dy = tr->dy * ONE;
        
        int lensq = (tr->dx*tr->dx + tr->dy*tr->dy); 
        int f_len = csqrt(lensq);
        tr->f_len = f_len;

        int f_angle = ratan2(f_dy, f_dx);
        tr->f_angle = f_angle;
        tr->f_dirx = ccos(f_angle);
        tr->f_diry = csin(f_angle);
        f_x += f_dx;
        f_y += f_dy;

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
        int f_trx0 = tr->f_x0;
        int f_try0 = tr->f_y0;
        int f_trx1 = tr->f_x0;
        int f_try1 = tr->f_y0;
        if (f_move > 0) {
            f_trx1 += ONE * tr->dx;
            f_try1 += ONE * tr->dy;
        } else {
            f_trx0 += ONE * tr->dx;
            f_try0 += ONE * tr->dy;
        }

        int distx = (f_trx1 - f_newx) / ONE;
        int disty = (f_try1 - f_newy) / ONE;
        int distsq = (distx * distx) + (disty * disty);
        int f_len = csqrt(distsq);
        int f_absmove = abs(f_move);
        if (f_absmove >= f_len) {
            f_newx = f_trx1;
            f_newy = f_try1;
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