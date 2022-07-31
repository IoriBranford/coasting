#include "car.h"

#include "draw.h"

struct Car {
    int f_x, f_y;
    int f_angle;
};

typedef struct Car Car;

Car car;

void car_setup() {
    car.f_x = car.f_y = 0;
    car.f_angle = 0;
}

void update_car() {
    car.f_angle += ONE/16;
}

void draw_car(short camerax, short cameray) {
    int f_screenx = car.f_x + camerax*ONE;
    int f_screeny = car.f_y + cameray*ONE;
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
        triangle[i]   = (tx * f_cos - ty * f_sin + f_screenx) / ONE;
        triangle[i+1] = (tx * f_sin + ty * f_cos + f_screeny) / ONE;
    }

	u_char colors[] = {
		255, 0, 0,
		0, 255, 0,
		0, 0, 255,
        0, 0, 0 // pad
	};
    draw_triangle_gouraud(triangle, colors);
}