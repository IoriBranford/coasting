#ifndef __TRACK_H
#define __TRACK_H

struct Track {
    short dx, dy;
    int x0, y0;
};

typedef struct Track Track;

void track_setup();
void move_on_tracks(int *f_x, int *f_y, int *f_pos, int *tri, int *f_angle, int f_speed);
void draw_tracks(short offsetx, short offsety);

#endif