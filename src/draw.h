#ifndef __draw_H
#define __draw_H

#include <types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header

typedef struct {
    u_char	r, g, b, _;
	short	x, y;
} ColorVertex;

void draw_setup(int region, int w, int h);

void set_clear_color(u_char r, u_char g, u_char b);
void set_draw_color(u_char r, u_char g, u_char b);

void draw_begin();

void draw_rect(short x, short y, short w, short h);
void draw_square16(ColorVertex *v);
void draw_square8(ColorVertex *v);

void draw_sprite(short x, short y, short w, short h, TIM_IMAGE *image, u_char u, u_char v);
void set_draw_sprite_page(TIM_IMAGE *image);

void draw_triangle_flat(short tri[]);
void draw_triangle_gouraud(short tri[], u_char colors[]);
void draw_quad_flat(ColorVertex v[]);
void draw_quad_gouraud(ColorVertex v[]);

void draw_line_flat(ColorVertex v[]);
void draw_line_gouraud(ColorVertex v[]);
void draw_2lines_gouraud(ColorVertex v[]);
void draw_3lines_gouraud(ColorVertex v[]);

void draw_end();

#endif