#ifndef __TRACK_H
#define __TRACK_H

struct Track {
    short dx, dy;
    int x0, y0;
    int f_start, f_len;
    int f_angle, f_dirx, f_diry;
};

typedef struct Track Track;

void track_setup();
void move_on_track(int *trackidx, int *f_position, int f_speed);
void track_set_transform(int *f_x, int *f_y, int *f_angle, int tri, int f_pos);
void draw_tracks(short offsetx, short offsety);

#endif