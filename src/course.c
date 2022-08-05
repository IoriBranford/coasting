#include "course.h"
#include "courses.h"

#include "draw.h"
#include "time.h"

#include <abs.h>

u_char COLORCYCLE[] = {
    255, 0, 255,
    0, 255, 255,
};

Course *course;

void select_course(int courseidx) {
    course = COURSES+courseidx;
}

int is_course_end(int f_pos) {
    return f_pos >= course->f_len;
}

int clamp_course_position(int f_pos) {
    if (f_pos < 0)
        f_pos = 0;
    if (f_pos > course->f_len)
        f_pos = course->f_len;
    return f_pos;
}

int clamp_track_index(int i) {
    if (i < 0)
        i = 0;
    if (i >= course->num_tracks)
        i = course->num_tracks-1;
    return i;
}

void move_on_course(int *trackidx, int *f_position, int f_speed) {
    int tri = *trackidx;
    int f_pos = *f_position;
    f_pos += f_speed;
    while (tri >= 0 && tri < course->num_tracks) {
        Track *tr = course->tracks + tri;
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
    Track *tr = course->tracks + tri;
    int posontrack = (f_pos - tr->f_start) / ONE;
    int f_dirx = tr->f_dirx;
    int f_diry = tr->f_diry;
    *x = tr->x0 + posontrack * f_dirx / ONE;
    *y = tr->y0 + posontrack * f_diry / ONE;
    *f_angle = tr->f_angle;
}

void draw_course() {
    int cn = sizeof(COLORCYCLE);
    int ci = ((get_time() / 6) * 3) % cn;
    Track *tr = course->tracks;
    ColorVertex vertices[2];
    for (int i = 0; i < course->num_tracks; ++i) {
        vertices->x = tr->x0;
        vertices->y = tr->y0;
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