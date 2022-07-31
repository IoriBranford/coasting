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
	// Load the internal font texture
	FntLoad(960, 0);
	// Create the text stream
	// FntOpen(8, 8, 304, 224, 0, 100);

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
		
        FntFlush(-1);
		draw_end();
		time_tick();
	}
	return 0;
}
