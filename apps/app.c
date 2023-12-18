#include <stdio.h>

#include "gpio_wrappers.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include "score_display.h"
#include "speaker.h"
#include "time_display.h"

#define RIGHT_WEAPON_PIN 6
#define RIGHT_JACKET_PIN 7
#define LEFT_JACKET_PIN 8
#define LEFT_WEAPON_PIN 9

#define RIGHT_YELLOW_LED 10
#define RIGHT_GREEN_LED 11
#define LEFT_RED_LED 12
#define LEFT_YELLOW_LED 13

volatile bool hit;
volatile bool hit_from_left_weapon;
volatile bool hit_left_jacket;
volatile bool hit_from_right_weapon;
volatile bool hit_right_jacket;

void enable_hits(void) {
    gpio_set_irq_enabled(LEFT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, true);
    gpio_set_irq_enabled(RIGHT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, true);
}

void disable_hits(void) {
    gpio_set_irq_enabled(LEFT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, false);
    gpio_set_irq_enabled(RIGHT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW, false);
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
    enable_hits();
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
    tone(400, 1200);
}

int64_t signal_hit_callback(alarm_id_t id, void *user_data) {
    if (!is_one_second_timer_stopped()) stop_one_second_timer();
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

void all_gpio_init(void) {
    gpio_out_init(LEFT_YELLOW_LED);
    gpio_out_init(LEFT_RED_LED);
    gpio_out_init(RIGHT_YELLOW_LED);
    gpio_out_init(RIGHT_GREEN_LED);

    gpio_out_init(SPEAKER);

    gpio_in_init(LEFT_JACKET_PIN);
    gpio_in_init(RIGHT_JACKET_PIN);
}

int main() {
    stdio_init_all();

    all_gpio_init();

    external_interrupt_init(LEFT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW,
                            &gpio_callback);
    external_interrupt_init(RIGHT_WEAPON_PIN, GPIO_IRQ_LEVEL_LOW,
                            &gpio_callback);

    time_display_init();
    reset_time();

    score_display_init();
    reset_score();

    // Wait forever
    while (1) {
        tight_loop_contents();
    }
}