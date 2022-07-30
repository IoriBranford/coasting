#include "draw.h"
#include "image.h"

int main() {
	short triangle[6];
	u_char colors[9] = {
		128, 0, 0,
		0, 128, 0,
		0, 0, 128
	};
	draw_setup(0, 320, 240);
	while (1) {
		draw_begin();
		for (int i = 16; i <= 160; i += 16) {
			triangle[0] = i; 	triangle[1] = i;
			triangle[2] = i+64; triangle[3] = i;
			triangle[4] = i+32; triangle[5] = i+64;

			draw_triangle_gouraud(triangle, colors);
			triangle[4] = i; 	triangle[5] = i+64;
			triangle[2] = i+64; triangle[3] = i+64;
			triangle[0] = i+32; triangle[1] = i;
			set_draw_color(i, i, 0);
			draw_triangle_flat(triangle);
			// draw_rect(i, i, 64, 64);
		}
		draw_end();
	}
	return 0;
}
