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

int f_courselength;
Track *course;
int coursesize;

int is_course_end(int f_pos) {
    return f_pos >= f_courselength;
}

void course_setup(int n, Track *c) {
    course = c;
    coursesize = n;
    Track *tr = c;
    int x = 0, y = 0;
    f_courselength = 0;
    for (int i = 0; i < n; ++i) {
        tr->x0 = x;
        tr->y0 = y;
        tr->f_start = f_courselength;
        int distx = tr->dx;
        int disty = tr->dy;
        tr->f_angle = ratan2(disty*ONE, distx*ONE);
        tr->f_dirx = ccos(tr->f_angle);
        tr->f_diry = csin(tr->f_angle);
        int f_len = 0;
        if (tr->f_dirx)
            f_len = ONE*distx / tr->f_dirx * ONE;
        if (!f_len && tr->f_diry)
            f_len = ONE*disty / tr->f_diry * ONE;
        tr->f_len = f_len;
        x += distx;
        y += disty;
        f_courselength += f_len;
        tr++;
    }
}

int clamp_course_position(int f_pos) {
    if (f_pos < 0)
        f_pos = 0;
    if (f_pos > f_courselength)
        f_pos = f_courselength;
    return f_pos;
}

int clamp_track_index(int i) {
    if (i < 0)
        i = 0;
    if (i >= coursesize)
        i = coursesize-1;
    return i;
}

void move_on_course(int *trackidx, int *f_position, int f_speed) {
    int tri = *trackidx;
    int f_pos = *f_position;
    f_pos += f_speed;
    while (tri >= 0 && tri < coursesize) {
        Track *tr = &course[tri];
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
    f_pos = clamp_course_position(f_pos);
    tri = clamp_track_index(tri);

    *trackidx = tri;
    *f_position = f_pos;
}

void course_transform_car(int *x, int *y, int *f_angle, int tri, int f_pos) {
    f_pos = clamp_course_position(f_pos);
    tri = clamp_track_index(tri);
    Track *tr = &course[tri];
    int posontrack = (f_pos - tr->f_start) / ONE;
    int f_dirx = tr->f_dirx;
    int f_diry = tr->f_diry;
    *x = tr->x0 + posontrack * f_dirx / ONE;
    *y = tr->y0 + posontrack * f_diry / ONE;
    *f_angle = tr->f_angle;
}

void draw_course(short offsetx, short offsety) {
    int cn = sizeof(COLORCYCLE);
    int ci = ((get_time() / 6) * 3) % cn;
    Track *tr = course;
    ColorVertex vertices[2];
    for (int i = 0; i < coursesize; ++i) {
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