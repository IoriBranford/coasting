#ifndef __TRACK_H
#define __TRACK_H

struct Track {
    short dx, dy;
    int f_x0, f_y0;
};

typedef struct Track Track;

void track_setup();
Track* get_track(int i);
void draw_tracks(short offsetx, short offsety);

#endif