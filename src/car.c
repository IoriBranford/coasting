#include "car.h"
#include "track.h"
#include "draw.h"
#include "input.h"

#include <abs.h>

#define MAX_FUEL 3600
#define DRIVE_FUEL_COST 3
#define F_DRIVE_FORCE (ONE/15)
#define F_GRAVITY     (ONE/30)
#define MAX_SPEED (ONE*10)

struct Car {
    int x, y;
    int f_coursepos;
    int trackidx;
    int f_angle;
    int f_speed;
    int f_accel;
    int fuel;
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
    car.fuel = MAX_FUEL;
}

void update_car() {
    Controller *controller = get_controller(0);
    if (car.fuel > 0 && is_button_pressed(controller, BUTTON_RIGHT)) {
        car.f_accel = F_DRIVE_FORCE;
        car.fuel -= DRIVE_FUEL_COST;
    } else if (car.fuel > 0 && is_button_pressed(controller, BUTTON_LEFT)) {
        car.f_accel = -F_DRIVE_FORCE;
        car.fuel -= DRIVE_FUEL_COST;
    } else {
        car.f_accel = 0;
        car.fuel--;
    }
    car.f_speed += car.f_accel;
    int f_gravity = (csin(car.f_angle)) * F_GRAVITY / ONE;
    car.f_speed += f_gravity;
    if ((car.f_speed < 0 && car.f_coursepos <= 0)
    || (car.f_speed > 0 && is_course_end(car.f_coursepos))) {
        car.f_speed = 0;
    }
    if (car.f_speed > MAX_SPEED)
        car.f_speed = MAX_SPEED;
    else if (car.f_speed < -MAX_SPEED)
        car.f_speed = -MAX_SPEED;

    move_on_course(&car.trackidx, &car.f_coursepos, car.f_speed);
    course_transform_car(&car.x, &car.y, &car.f_angle, car.trackidx, car.f_coursepos);
}

void car_set_camera(short *camerax, short *cameray) {
    int x = car.x, y = car.y;
    *camerax = 160-x;
    *cameray = 120-y;
}

void draw_car() {
    int screenx = car.x;
    int screeny = car.y;
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

ColorVertex fuelgauge[] = {
    {.x = 8, .y = 60,   .r = 0xe5, .g = 0x9b, .b = 0x30},
    {.x = 16, .y = 60,  .r = 0xe5, .g = 0x9b, .b = 0x30},
    {.x = 8, .y = 180,  .r = 0xe4, .g =  0x3, .b = 0x95},
    {.x = 16, .y = 180, .r = 0xe4, .g =  0x3, .b = 0x95},
};

ColorVertex speedgauge[] = {
    {.x = 80, .y = 224,  .r = 0xff, .g =  0, .b = 0xff},//#E40395
    {.x = 80, .y = 232,  .r = 0xff, .g =  0, .b = 0xff},
    {.x = 240, .y = 224, .r = 0, .g = 0xff, .b = 0xff},
    {.x = 240, .y = 232, .r = 0, .g = 0xff, .b = 0xff},
};

void draw_hud() {
    set_draw_area(0);

    int fuelheight = car.fuel * 120 / MAX_FUEL;
    RECT gaugerect = {0, 60+120-fuelheight, 320, fuelheight};
    draw_quad_gouraud(fuelgauge);
    set_draw_area(&gaugerect);
}

void draw_speed_gauge() {
    RECT gaugerect;
    int speedwidth = car.f_speed * 160 / MAX_SPEED;
    gaugerect.x = 80; gaugerect.y = 0;
    gaugerect.w = speedwidth; gaugerect.h = 240;
    draw_quad_gouraud(speedgauge);
    set_draw_area(&gaugerect);
}