#ifndef _TIME_DISPLAY_H
#define _TIME_DISPLAY_H

#include <stdio.h>

#include "pico/stdlib.h"

bool is_one_second_timer_stopped(void);
void start_one_second_timer(void);
void stop_one_second_timer(void);
void start_stop_one_second_timer(void);
bool one_second_timer_callback(struct repeating_timer *);
void time_display_init(void);
void reset_time(void);
void one_minute_break(void);

#endif