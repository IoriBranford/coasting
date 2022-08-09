#include "car.h"
#include "course.h"
#include "draw.h"
#include "input.h"
#include "time.h"
#include "particle.h"

#include <abs.h>
#include <rand.h>

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

    int f_partspeed = car.f_speed * (rand()%ONE) / ONE;
    if (f_partspeed) {
        int f_angle = car.f_angle - ONE/2 + (rand()%(ONE/6)) - ONE/12;
        int f_partvelx = ccos(f_angle) * f_partspeed / ONE;
        int f_partvely = csin(f_angle) * f_partspeed / ONE;
        ColorVertex v = {
            .x = car.x + f_partspeed/ONE, .y = car.y + f_partspeed/ONE, .r = 0, .g = 255, .b = 255
        };
        u_int time = abs(f_partspeed)*10/ONE;
        add_particle(&v, f_partvelx, f_partvely, time);
    }

    move_on_course(&car.trackidx, &car.f_coursepos, car.f_speed);
    course_transform_car(&car.x, &car.y, &car.f_angle, car.trackidx, car.f_coursepos);
}

int car_finished_course() {
    return is_course_end(car.f_coursepos);
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
    ColorVertex v[3] = {
        {.x = 0, .y = 0,   .r = 0xe4, .g =  0x3, .b = 0x95},
        {.x = 0, .y = -16, .r = 0xe4, .g =  0x3, .b = 0x95},
        {.x = 16, .y = -8, .r = 0xe5, .g = 0x9b, .b = 0x30},
    };

    for (int i = 0; i < 3; ++i) {
        int vx = v[i].x;
        int vy = v[i].y;
        v[i].x = (vx * f_cos - vy * f_sin) / ONE + screenx;
        v[i].y = (vx * f_sin + vy * f_cos) / ONE + screeny;
    }

    draw_triangle_gouraud(v);
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