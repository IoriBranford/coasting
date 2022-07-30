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

void draw_end() {
    DrawSync(0);                // Wait for any draw processing to finish
    
    VSync(0);                   // Wait for vertical retrace

    PutDispEnv(&disp[db]);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv(&draw[db]);

    SetDispMask(1);             // Enable the display

    DrawOTag(ot[db]+OTLEN-1);   // Draw the ordering table
    
    db = !db;                   // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db];      // Reset next primitive pointer
}
