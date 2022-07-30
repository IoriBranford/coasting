#ifndef __draw_H
#define __draw_H

#include <sys/types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header

void draw_setup(int region, int w, int h);

void set_clear_color(u_char r, u_char g, u_char b);
void set_draw_color(u_char r, u_char g, u_char b);

void draw_begin();

void draw_rect(short x, short y, short w, short h);

void draw_sprite(short x, short y, short w, short h, TIM_IMAGE *image, u_char u, u_char v);
void set_draw_sprite_page(TIM_IMAGE *image);

void draw_end();

#endif