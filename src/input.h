#ifndef __INPUT_H
#define __INPUT_H

#include <types.h>

#define NUM_CONTROLLERS 2

// Pad stuff (omit when using PSn00bSDK)
#define BUTTON_SELECT      1
#define BUTTON_L3          2
#define BUTTON_R3          4
#define BUTTON_START       8
#define BUTTON_UP          16
#define BUTTON_RIGHT       32
#define BUTTON_DOWN        64
#define BUTTON_LEFT        128
#define BUTTON_L2          256
#define BUTTON_R2          512
#define BUTTON_L1          1024
#define BUTTON_R1          2048
#define BUTTON_TRIANGLE    4096
#define BUTTON_CIRCLE      8192
#define BUTTON_CROSS       16384
#define BUTTON_SQUARE      32768

typedef struct Controller Controller;

void input_setup();
Controller* get_controller(u_int i);
int is_button_pressed(Controller *controller, u_short button);

#endif