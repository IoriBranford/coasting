#include "input.h"

#include <libapi.h>     // API header, has InitPAD() and StartPAD() defs

struct Controller
{
    unsigned char	state;
    unsigned char	len:4;
    unsigned char	type:4;
    unsigned short	buttons;
    unsigned char	rsx,rsy;
    unsigned char	lsx,lsy;
};

// pad buffer arrays
char padbuff[NUM_CONTROLLERS][34];

void input_setup() {
    // Initialize the pads
    InitPAD( padbuff[0], 34, padbuff[1], 34 );
    
    // Begin polling
    StartPAD();
    
    // To avoid VSync Timeout error, may not be defined in PsyQ
    ChangeClearPAD( 1 );
}

Controller* get_controller(u_int i) {
    if (i >= NUM_CONTROLLERS)
        return NULL;
    Controller *controller = (Controller*)&padbuff[i];
    return controller->state == 0 ? controller : NULL;
}

int is_button_pressed(Controller *controller, u_short button) {
    return !(controller->buttons & button);
}