#include "draw.h"
#include "image.h"
#include "input.h"
#include "track.h"
#include "car.h"
#include "time.h"
#include "audio.h"

int main() {
	short camerax = 60, cameray = 120;
	Controller *controller;

	draw_setup(0, 320, 240);
	set_clear_color(32, 0, 32);
	audio_setup();
	input_setup();
	controller = get_controller(0);

	track_setup();
	car_setup();

	play_cdda(2, 1);

	while (1) {
		if (is_button_pressed(controller, BUTTON_RIGHT))
			camerax--;
		if (is_button_pressed(controller, BUTTON_LEFT))
			camerax++;

		update_car();

		draw_begin();
		draw_car(camerax, cameray);
		draw_tracks(camerax, cameray);
		draw_end();
		time_tick();
	}
	return 0;
}
