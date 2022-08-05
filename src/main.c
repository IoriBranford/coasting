#include "draw.h"
#include "image.h"
#include "input.h"
#include "track.h"
#include "car.h"
#include "time.h"
#include "audio.h"
#include "background.h"
#include "courses.h"
#include "particle.h"

int main() {
	short camera[2];

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
	clear_particles();

	play_cdda(2, 1);
	while (1) {
		update_car();
		update_particles();

		car_set_camera(camera, camera+1);
		draw_begin();

		draw_hud();
		set_draw_offset(0);

		draw_particles();
		draw_car();
		draw_course();
		set_draw_offset(camera);

		draw_background(camera[0], camera[1]);

        FntFlush(-1);
		draw_end();

		time_tick();
	}
	return 0;
}
