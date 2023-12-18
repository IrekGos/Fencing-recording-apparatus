#ifndef _GPIO_WRAPPERS_H
#define _GPIO_WRAPPERS_H

#include <stdio.h>

void gpio_out_init(uint);
void gpio_in_init(uint);
void external_interrupt_init(uint, uint32_t, void *);

#endif