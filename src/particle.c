#include "draw.h"

struct Particle {
    int f_x, f_y;
    short f_velx, f_vely;
    u_char time, r, g, b;
};
typedef struct Particle Particle;

#define MAX_PARTICLES 256

Particle particles[MAX_PARTICLES];

void clear_particles() {
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        particles[i].time = 0;
    }
}

void add_particle(ColorVertex *v, short f_velx, short f_vely, u_int time) {
    Particle *p = 0;
    for (int i = 0; !p && i < MAX_PARTICLES; ++i) {
        if (!particles[i].time) {
            p = &particles[i];
        }
    }

    if (!p)
        return;
    p->r = v->r;
    p->g = v->g;
    p->b = v->b;
    p->f_x = ONE*v->x;
    p->f_y = ONE*v->y;
    p->f_velx = f_velx;
    p->f_vely = f_vely;
    p->time = time > 255? 255 : time;
}

void update_particles() {
    Particle *p = particles;
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (p->time) {
            p->time--;
            p->f_x += p->f_velx;
            p->f_y += p->f_vely;
        }
        ++p;
    }
}

void draw_particles() {
    Particle *p = particles;
    ColorVertex v;
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (p->time) {
            v.r = p->r;
            v.g = p->g;
            v.b = p->b;
            v.x = p->f_x / ONE;
            v.y = p->f_y / ONE;
            draw_pixel(&v);
        }
        ++p;
    }
}