#include "draw.h"
#include "image.h"

int main() {
	draw_setup(0, 320, 240);
	while (1) {
		draw_begin();
		for (int i = 16; i <= 160; i += 16) {
			set_draw_color(i, i, 0);
			draw_rect(i, i, 64, 64);
		}
		draw_end();
	}
	return 0;
}
