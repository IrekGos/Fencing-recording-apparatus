#include "time_display.h"

#include "PicoTM1637.h"
#include "speaker.h"

volatile uint8_t minutes, seconds;

struct repeating_timer one_second_timer;

bool is_one_second_timer_stopped(void) { return !one_second_timer.alarm_id; }

void start_one_second_timer(void) {
    add_repeating_timer_ms(1000, one_second_timer_callback, NULL,
                           &one_second_timer);
}

void stop_one_second_timer(void) { cancel_repeating_timer(&one_second_timer); }

void start_stop_one_second_timer(void) {
    if (is_one_second_timer_stopped())
        start_one_second_timer();
    else
        stop_one_second_timer();
}

bool one_second_timer_callback(struct repeating_timer *timer) {
    if (seconds != 0) {
        seconds--;
        TM1637_display_right(seconds, true);
    } else if (minutes != 0) {
        seconds = 59;
        minutes--;
        TM1637_display_both(minutes, seconds, true);
    } else {
        if (!is_muted()) tone(400, 1200);
        stop_one_second_timer();
    }
    // return true to continue repeating
    return true;
}

void time_display_init(void) {
    TM1637_init(PICO_DEFAULT_I2C_SCL_PIN, PICO_DEFAULT_I2C_SDA_PIN);
    // 7 is a maximum brightness value
    TM1637_set_brightness(7);
}

void reset_time(void) {
    stop_one_second_timer();
    minutes = 3, seconds = 0;
    TM1637_display_both(minutes, seconds, true);
}

void one_minute_break(void) {
    minutes = 1, seconds = 0;
    TM1637_display_both(minutes, seconds, true);
    start_one_second_timer();
}