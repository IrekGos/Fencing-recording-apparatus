#include "gpio_wrappers.h"

#include "pico/stdlib.h"

void gpio_out_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_OUT);
}

void gpio_in_init(uint gpio) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_down(gpio);
}

void external_interrupt_init(uint gpio, uint32_t mask, void *gpio_callback) {
    gpio_in_init(gpio);
    gpio_set_irq_enabled_with_callback(gpio, mask, true, gpio_callback);
}