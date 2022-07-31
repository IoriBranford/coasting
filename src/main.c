#include "draw.h"
#include "image.h"
#include "input.h"
#include "track.h"
#include "time.h"

int main() {
	short x = 0, y = 0;
	short triangle[6];
	u_char colors[9] = {
		128, 0, 0,
		0, 128, 0,
		0, 0, 128
	};
	Controller *controller;

	draw_setup(0, 320, 240);
	set_clear_color(32, 0, 32);
	input_setup();
	controller = get_controller(0);

	while (1) {
		if (is_button_pressed(controller, BUTTON_RIGHT))
			x++;
		if (is_button_pressed(controller, BUTTON_LEFT))
			x--;

		draw_begin();

		triangle[0] = x; triangle[1] = y;
		triangle[2] = x + 16; triangle[3] = y+8;
		triangle[4] = x; triangle[5] = y+16;
		draw_triangle_gouraud(triangle, colors);

		draw_tracks();
		// for (int i = 16; i <= 160; i += 16) {
		// 	triangle[0] = i; 	triangle[1] = i;
		// 	triangle[2] = i+64; triangle[3] = i;
		// 	triangle[4] = i+32; triangle[5] = i+64;

		// 	draw_triangle_gouraud(triangle, colors);
		// 	triangle[4] = i; 	triangle[5] = i+64;
		// 	triangle[2] = i+64; triangle[3] = i+64;
		// 	triangle[0] = i+32; triangle[1] = i;
		// 	set_draw_color(i, i, 0);
		// 	draw_triangle_flat(triangle);
		// 	// draw_rect(i, i, 64, 64);
		// }
		draw_end();
		time_tick();
	}
	return 0;
}
