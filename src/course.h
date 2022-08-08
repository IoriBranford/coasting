#ifndef __COURSE_H
#define __COURSE_H

typedef struct Track {
    short dx, dy;
    int x0, y0;
    int f_start, f_len;
    int f_angle, f_dirx, f_diry;
} Track;

typedef struct Course {
    char *name;
    int f_len;
    int num_tracks;
    Track *tracks;
} Course;

void select_course(int courseidx);
int is_course_end(int f_pos);
void move_on_course(int *trackidx, int *f_position, int f_speed);
void course_transform_car(int *x, int *y, int *f_angle, int tri, int f_pos);
void draw_course();
void draw_exit();

#endif