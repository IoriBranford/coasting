#include "time.h"

static unsigned long time = 0;

unsigned long get_time() {
    return time;
}

void time_tick() {
    time++;
}