#ifndef _SPEAKER_H
#define _SPEAKER_H

#include <stdio.h>

#include "pico/stdlib.h"

#define SPEAKER 15

bool is_muted(void);
void mute_unmute(void);
void toggle(uint);
void tone(uint32_t, uint32_t);

#endif