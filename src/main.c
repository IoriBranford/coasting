#include "draw.h"
#include "image.h"
#include "input.h"
#include "track.h"
#include "car.h"
#include "time.h"
#include "audio.h"
#include "background.h"
#include "courses.h"

int main() {
	short camerax = 60, cameray = 120;

	draw_setup(0, 320, 240);
	set_clear_color(32, 0, 32);
	// Load the internal font texture
	FntLoad(960, 0);
	// Create the text stream
	// FntOpen(8, 8, 304, 224, 0, 100);

	audio_setup();
	input_setup();

	course_setup(COURSE_0_SIZE, COURSE_0);
	car_setup();

	play_cdda(2, 1);

	while (1) {
		update_car();
		car_set_camera(&camerax, &cameray);
		draw_begin();
		draw_hud();
		draw_car(camerax, cameray);
		draw_course(camerax, cameray);
		draw_background(camerax, cameray);
        FntFlush(-1);
		draw_end();
		time_tick();
	}
	return 0;
}
