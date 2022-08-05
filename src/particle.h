#ifndef __PARTICLE_H
#define __PARTICLE_H

typedef struct ColorVertex ColorVertex;

void clear_particles();
void add_particle(ColorVertex *v, short f_velx, short f_vely, unsigned int time);
void update_particles();
void draw_particles();

#endif