#include "draw.h"
#include "image.h"

#include <libetc.h>	// Includes some functions that controls the display

#define OTLEN 8         // Ordering table length (recommended to set as a define
                        // so it can be changed easily)

DISPENV disp[2];
DRAWENV draw[2];
int db = 0;

u_long ot[2][OTLEN];    // Ordering table length
char pribuff[2][32768]; // Primitive buffer
char *nextpri;          // Next primitive pointer

u_char drawcolor[4];

void draw_setup(int region, int w, int h) {
    // Reset draw
    ResetGraph(region);

    for (int i = 0; i < 4; ++i)
        drawcolor[i] = 128;

    // First buffer
    SetDefDispEnv(&disp[0], 0, 0, w, h);
    SetDefDrawEnv(&draw[0], 0, h, w, h);
    // Second buffer
    SetDefDispEnv(&disp[1], 0, h, w, h);
    SetDefDrawEnv(&draw[1], 0, 0, w, h);

    SetDispMask(1);             // Enable the display

    draw[0].isbg = 1;               // Enable clear
    draw[1].isbg = 1;
    set_clear_color(128, 128, 128);

    nextpri = pribuff[0];           // Set initial primitive pointer address
}

void set_clear_color(u_char r, u_char g, u_char b) {
    setRGB0(&draw[0], r, g, b);  // Set clear color (dark purple)
    setRGB0(&draw[1], r, g, b);
}

void set_draw_color(u_char r, u_char g, u_char b) {
    drawcolor[0] = r;
    drawcolor[1] = g;
    drawcolor[2] = b;
}

void draw_begin() {
    ClearOTagR(ot[db], OTLEN);  // Clear ordering table
}

void draw_rect(short x, short y, short w, short h) {
    TILE *tile = (TILE*)nextpri;      // Cast next primitive

    setTile(tile);              // Initialize the primitive (very important)
    setXY0(tile, x, y);       // Set primitive (x,y) position
    setWH(tile, w, h);        // Set primitive size
    setRGB0(tile, drawcolor[0], drawcolor[1], drawcolor[2]); 
    addPrim(ot[db], tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

void draw_square16(ColorVertex *v) {
    TILE_16 *tile = (TILE_16*)nextpri;      // Cast next primitive

    setTile16(tile);              // Initialize the primitive (very important)
    setXY0(tile, v->x, v->y);       // Set primitive (x,y) position
    setRGB0(tile, v->r, v->g, v->b); 
    addPrim(ot[db], tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE_16);    // Advance the next primitive pointer
}

void draw_square8(ColorVertex *v) {
    TILE_8 *tile = (TILE_8*)nextpri;      // Cast next primitive

    setTile8(tile);              // Initialize the primitive (very important)
    setXY0(tile, v->x, v->y);       // Set primitive (x,y) position
    setRGB0(tile, v->r, v->g, v->b); 
    addPrim(ot[db], tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE_8);    // Advance the next primitive pointer
}

void draw_sprite(short x, short y, short w, short h, TIM_IMAGE *image, u_char u, u_char v) {
    SPRT *sprt = (SPRT *)nextpri;
    setSprt(sprt);
    setRGB0(sprt, drawcolor[0], drawcolor[1], drawcolor[2]);        
    setXY0(sprt, x, y);
    setUV0(sprt, u, v);
    setWH(sprt, w, h);
    if (is_indexed_image(image)) {
        setClut(sprt, image->crect->x, image->crect->y);
    }
    addPrim(ot[db], sprt);
    nextpri += sizeof(SPRT);
}

void set_draw_sprite_page(TIM_IMAGE *image) {
    DR_TPAGE *tpage = (DR_TPAGE*)nextpri;
    setDrawTPage(tpage, 0, 1,               
        getTPage(image->mode&0x3, 0,            
        image->prect->x, image->prect->y));
    addPrim(ot[db], tpage);                 
    nextpri += sizeof(DR_TPAGE);
}

void draw_triangle_flat(short tri[]) {
    POLY_F3 *poly = (POLY_F3*)nextpri;
    setPolyF3(poly);
    setRGB0(poly, drawcolor[0], drawcolor[1], drawcolor[2]);
    setXY3(poly, tri[0], tri[1], tri[2], tri[3], tri[4], tri[5]);
    addPrim(ot[db], poly);
    nextpri += sizeof(POLY_F3);
}

void draw_triangle_gouraud(short tri[], u_char colors[]) {
    POLY_G3 *poly = (POLY_G3*)nextpri;
    setPolyG3(poly);
    setRGB0(poly, colors[0], colors[1], colors[2]);
    setRGB1(poly, colors[3], colors[4], colors[5]);
    setRGB2(poly, colors[6], colors[7], colors[8]);
    setXY3(poly, tri[0], tri[1], tri[2], tri[3], tri[4], tri[5]);
    addPrim(ot[db], poly);
    nextpri += sizeof(POLY_G3);
}

void draw_3lines_gouraud(ColorVertex v[]) {
    LINE_G4 *line = (LINE_G4*)nextpri;
    setLineG4(line);
    setRGB0(line, v->r, v->g, v->b);
    setRGB1(line, v[1].r, v[1].g, v[1].b);
    setRGB2(line, v[2].r, v[2].g, v[2].b);
    setRGB3(line, v[3].r, v[3].g, v[3].b);
    setXY4(line, v->x, v->y, v[1].x, v[1].y, v[2].x, v[2].y, v[3].x, v[3].y);
    addPrim(ot[db], line);
    nextpri += sizeof(LINE_G4);
}

void draw_end() {
    DrawSync(0);                // Wait for any draw processing to finish
    
    VSync(0);                   // Wait for vertical retrace

    PutDispEnv(&disp[db]);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw[db]);

    DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table
    
    db = !db;                   // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db];      // Reset next primitive pointer
}
