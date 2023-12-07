#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define RIGHT_WEAPON_PIN 6
#define RIGHT_JACKET_PIN 7
#define LEFT_JACKET_PIN 8
#define LEFT_WEAPON_PIN 9

#define RIGHT_YELLOW_LED 10
#define RIGHT_GREEN_LED 11
#define LEFT_RED_LED 12
#define LEFT_YELLOW_LED 13

#define SPEAKER 15

volatile bool hit;
volatile bool hit_from_left_weapon;
volatile bool hit_left_jacket;
volatile bool hit_from_right_weapon;
volatile bool hit_right_jacket;

struct repeating_timer signal_hit_timer;

void toggle(uint gpio) { gpio_put(gpio, !gpio_get(gpio)); }

bool repeating_timer_callback(struct repeating_timer *timer) {
    toggle(SPEAKER);
    if ((u_int32_t)timer->user_data-- == 0) cancel_repeating_timer(timer);
    return true;
}

void tone(uint32_t step, uint32_t delay, struct repeating_timer *timer) {
    add_repeating_timer_us(step, repeating_timer_callback, (void *)delay,
                           timer);
}

void TONE(uint32_t step, uint32_t delay) {
    for (uint16_t i = 0; i < (uint32_t)1000 * (delay) / (step); i++) {
        toggle(SPEAKER);
        busy_wait_us(step);
    }
}

void check_hit(uint gpio) {
    switch (gpio) {
        case RIGHT_WEAPON_PIN:
            hit_from_right_weapon = true;
            hit_left_jacket = gpio_get(LEFT_JACKET_PIN);
            break;
        case LEFT_WEAPON_PIN:
            hit_from_left_weapon = true;
            hit_right_jacket = gpio_get(RIGHT_JACKET_PIN);
            break;
        default:
            break;
    }
}

int64_t turn_off_leds_callback(alarm_id_t id, void *user_data) {
    uint32_t mask = 1 << LEFT_RED_LED | 1 << RIGHT_GREEN_LED |
                    1 << RIGHT_YELLOW_LED | 1 << LEFT_YELLOW_LED;
    gpio_clr_mask(mask);
    // Can return a value here in us to fire in the future
    return 0;
}

int64_t enable_hits_callback(alarm_id_t id, void *user_data) {
    gpio_set_irq_enabled(LEFT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, true);
    gpio_set_irq_enabled(RIGHT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, true);
    // Can return a value here in us to fire in the future
    return 0;
}

void signal_hit(void) {
    if (hit_from_left_weapon && hit_right_jacket)
        gpio_put(LEFT_RED_LED, 1);
    else if (hit_from_left_weapon)
        gpio_put(LEFT_YELLOW_LED, 1);
    if (hit_from_right_weapon && hit_left_jacket)
        gpio_put(RIGHT_GREEN_LED, 1);
    else if (hit_from_right_weapon)
        gpio_put(RIGHT_YELLOW_LED, 1);
    hit_from_left_weapon = hit_from_right_weapon = hit_left_jacket =
        hit_right_jacket = false;
    add_alarm_in_ms(1500, turn_off_leds_callback, NULL, false);
    add_alarm_in_ms(1500, enable_hits_callback, NULL, false);
    TONE(400, 1200);
}

int64_t signal_hit_callback(alarm_id_t id, void *user_data) {
    hit = false;
    signal_hit();
    // Can return a value here in us to fire in the future
    return 0;
}

void gpio_callback(uint gpio, uint32_t events) {
    // Disabling gpio interrupts is necessary. Otherwise, gpio will call one
    // interrupt after another and block the device
    gpio_set_irq_enabled(gpio, events, false);
    if (!hit) {
        add_alarm_in_ms(50, signal_hit_callback, NULL, false);
        hit = true;
    }
    check_hit(gpio);
}

void gpio_out_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

void gpio_in_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_down(gpio);
}

void external_interrupt_init(uint gpio, uint32_t mask) {
    gpio_in_init(gpio);
    gpio_set_irq_enabled_with_callback(gpio, mask, true, &gpio_callback);
}

int main() {
    stdio_init_all();

    gpio_out_init(LEFT_YELLOW_LED);
    gpio_out_init(LEFT_RED_LED);
    gpio_out_init(RIGHT_YELLOW_LED);
    gpio_out_init(RIGHT_GREEN_LED);

    gpio_out_init(SPEAKER);

    gpio_in_init(LEFT_JACKET_PIN);
    gpio_in_init(RIGHT_JACKET_PIN);

    external_interrupt_init(LEFT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW);
    external_interrupt_init(RIGHT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW);

    // Wait forever
    while (1) {
        tight_loop_contents();
    }
}