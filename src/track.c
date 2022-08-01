#include "track.h"

#include "draw.h"
#include "time.h"

#include <abs.h>

Track TRACKS[] = {
    {64, 0},

    {15, 1},
    {14, 2},
    {13, 3},
    {12, 4},

    {11, 5},
    {10, 6},
    {9, 7},
    {8, 8},

    {8, 8},
    {9, 7},
    {10, 6},
    {11, 5},

    {12, 4},
    {13, 3},
    {14, 2},
    {15, 1},

    {64, 0},

    {15, -1},
    {14, -2},
    {13, -3},
    {12, -4},

    {11, -5},
    {10, -6},
    {9,  -7},
    {8, -8},

    {8, -8},
    {9,  -7},
    {10, -6},
    {11, -5},

    {12, -4},
    {13, -3},
    {14, -2},
    {15, -1},

    {64, 0},

    {15, -1},
    {14, -2},
    {13, -3},
    {12, -4},
    {11, -5},
    {10, -6},
    {9,  -7},
    {8,  -8},
    {7, -9},
    {6, -10},
    {5, -11},
    {4, -12},
    {3, -13},
    {2, -14},
    {1, -15},
    {0, -16},
    {-1,  -15},
    {-2,  -14},
    {-3,  -13},
    {-4,  -12},
    {-5,  -11},
    {-6,  -10},
    {-7,  -9},
    {-8,  -8},
    {-9,  -7},
    {-10, -6},
    {-11, -5},
    {-12, -4},
    {-13, -3},
    {-14, -2},
    {-15, -1},
    {-16, 0},
    {-15, 1},
    {-14, 2},
    {-13, 3},
    {-12, 4},
    {-11, 5},
    {-10, 6},
    {-9,  7},
    {-8,  8},
    {-7,  9},
    {-6,  10},
    {-5,  11},
    {-4,  12},
    {-3,  13},
    {-2,  14},
    {-1,  15},
    {0,  16},
    {1,  15},
    {2,  14},
    {3,  13},
    {4,  12},
    {5,  11},
    {6,  10},
    {7,  9},
    {8,  8},
    {9,  7},
    {10, 6},
    {11, 5},
    {12, 4},
    {13, 3},
    {14, 2},
    {15, 1},
    {16, 0},
    
    {64, 0},
};
const int NUM_TRACKS = sizeof(TRACKS)/sizeof(Track);

u_char COLORCYCLE[] = {
    255, 0, 255,
    0, 255, 255,
};

int f_totallen;

void track_setup() {
    Track *tr = TRACKS;
    int x = 0, y = 0;
    f_totallen = 0;
    for (int i = 0; i < NUM_TRACKS; ++i) {
        tr->x0 = x;
        tr->y0 = y;
        tr->f_start = f_totallen;
        int distx = tr->dx;
        int disty = tr->dy;
        tr->f_angle = ratan2(disty*ONE, distx*ONE);
        tr->f_dirx = ccos(tr->f_angle);
        tr->f_diry = csin(tr->f_angle);
        int f_len = ONE*distx / tr->f_dirx*ONE;
        tr->f_len = f_len;
        x += distx;
        y += disty;
        f_totallen += f_len;
        tr++;
    }
}

void move_on_tracks(int *f_x, int *f_y, int *f_pos, int *tri, int *f_angle, int f_speed) {
    int f_newx = *f_x, f_newy = *f_y;
    int f_newpos = *f_pos;
    int newtri = *tri;
    int f_move = f_speed;

    while (newtri >= 0 && newtri < NUM_TRACKS && f_move) {
        Track *tr = &TRACKS[newtri];
        int f_destx = tr->x0 * ONE;
        int f_desty = tr->y0 * ONE;
        int dir = -1;
        if (f_move > 0) {
            dir = 1;
            f_destx += ONE * tr->dx;
            f_desty += ONE * tr->dy;
        }

        int f_distx = (f_destx - f_newx);
        int f_disty = (f_desty - f_newy);
        int f_movex = f_distx;
        int f_movey = f_disty;
        if (f_distx || f_disty) {
            int f_moveangle = ratan2(f_disty, f_distx);
            *f_angle = f_moveangle;
            if (f_move < 0)
                *f_angle += ONE/2;
            f_movex = abs(f_move) * ccos(f_moveangle) / ONE;
            f_movey = abs(f_move) * csin(f_moveangle) / ONE;
        }

        int distx = f_distx / ONE;
        int disty = f_disty / ONE;
        int distsq = (distx * distx) + (disty * disty);
        int f_len = csqrt(distsq*ONE);

        if (abs(f_move) >= f_len) {
            f_newx = f_destx;
            f_newy = f_desty;
            int f_deltapos = dir*f_len;
            f_move -= f_deltapos;
            f_newpos += f_deltapos;
            newtri += dir;
        } else {
            f_newx += f_movex;
            f_newy += f_movey;
            f_newpos += f_move;
            f_move = 0;
        }
    }
    if (newtri < 0)
        newtri = 0;
    if (newtri >= NUM_TRACKS)
        newtri = NUM_TRACKS-1;
    *f_x = f_newx;
    *f_y = f_newy;
    *f_pos = f_newpos;
    *tri = newtri;
}

void move_on_track(int *trackidx, int *f_position, int f_speed) {
    int tri = *trackidx;
    int f_pos = *f_position;
    f_pos += f_speed;
    while (tri >= 0 && tri < NUM_TRACKS) {
        Track *tr = &TRACKS[tri];
        int f_dest = tr->f_start;
        if (f_speed > 0) {
            f_dest += tr->f_len;
            if (f_pos >= f_dest)
                ++tri;
            else
                break;
        } else {
            if (f_pos <= f_dest)
                --tri;
            else
                break;
        }
    }
    if (f_pos < 0)
        f_pos = 0;
    if (f_pos > f_totallen)
        f_pos = f_totallen;
    if (tri < 0)
        tri = 0;
    if (tri >= NUM_TRACKS)
        tri = NUM_TRACKS-1;

    *trackidx = tri;
    *f_position = f_pos;
}

void track_set_transform(int *f_x, int *f_y, int *f_angle, int tri, int f_pos) {
    Track *tr = &TRACKS[tri];
    int posontrack = (f_pos - tr->f_start) / ONE;
    int f_trx0 = ONE*tr->x0;
    int f_try0 = ONE*tr->y0;
    int f_dirx = tr->f_dirx;
    int f_diry = tr->f_diry;
    *f_x = f_trx0 + posontrack * f_dirx;
    *f_y = f_try0 + posontrack * f_diry;
    *f_angle = tr->f_angle;
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