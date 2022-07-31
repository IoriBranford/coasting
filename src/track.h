#ifndef __TRACK_H
#define __TRACK_H

struct Track {
    short dx, dy;
};

typedef struct Track Track;

Track* get_track(int i);
void draw_tracks();

#endif