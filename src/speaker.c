#include "speaker.h"

#include "pico/stdlib.h"

void toggle(uint gpio) { gpio_put(gpio, !gpio_get(gpio)); }

void tone(uint32_t step, uint32_t delay) {
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step); i++) {
        toggle(SPEAKER);
        busy_wait_us(step);
    }
}