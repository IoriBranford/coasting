#include "car.h"
#include "track.h"
#include "draw.h"
#include "input.h"

#include <abs.h>

struct Car {
    int x, y;
    int f_coursepos;
    int trackidx;
    int f_angle;
    int f_speed;
    int f_accel;
};

typedef struct Car Car;

Car car;

void car_setup() {
    car.x = car.y = 0;
    car.f_coursepos = 0;
    car.trackidx = 0;
    car.f_angle = 0;
    car.f_speed = 0;
    car.f_accel = 0;
}

#define F_DRIVE_FORCE (ONE/15)
#define F_GRAVITY     (ONE/30)

void update_car() {
    Controller *controller = get_controller(0);
    if (is_button_pressed(controller, BUTTON_RIGHT)) {
        car.f_accel = F_DRIVE_FORCE;
    } else if (is_button_pressed(controller, BUTTON_LEFT)) {
        car.f_accel = -F_DRIVE_FORCE;
    } else {
        car.f_accel = 0;
    }
    car.f_speed += car.f_accel;
    int f_gravity = (csin(car.f_angle)) * F_GRAVITY / ONE;
    car.f_speed += f_gravity;
    if ((car.f_speed < 0 && car.f_coursepos <= 0)
    || (car.f_speed > 0 && is_course_end(car.f_coursepos))) {
        car.f_speed = 0;
    }
    move_on_track(&car.trackidx, &car.f_coursepos, car.f_speed);
    track_set_transform(&car.x, &car.y, &car.f_angle, car.trackidx, car.f_coursepos);
}

void car_set_camera(short *camerax, short *cameray) {
    int x = car.x, y = car.y, _;
    *camerax = 160-x;
    *cameray = 120-y;
}

void draw_car(short camerax, short cameray) {
    int screenx = car.x + camerax;
    int screeny = car.y + cameray;
    int f_cos = ccos(car.f_angle);
    int f_sin = csin(car.f_angle);
	short triangle[] = {
        0, 0,
        0, -16,
        16, -8
    };
    for (int i = 0; i < 6; i += 2) {
        int tx = triangle[i];
        int ty = triangle[i+1];
        triangle[i]   = (tx * f_cos - ty * f_sin) / ONE + screenx;
        triangle[i+1] = (tx * f_sin + ty * f_cos) / ONE + screeny;
    }

	u_char colors[] = {
		255, 0, 0,
		0, 255, 0,
		0, 0, 255,
        0, 0, 0 // pad
	};
    draw_triangle_gouraud(triangle, colors);
}