#ifndef _SCORE_DISPLAY_H
#define _SCORE_DISPLAY_H

#include <stdio.h>

#define LEFT 0
#define RIGHT 1
#define DOWN 0
#define UP 1

void score_display_init(void);
void reset_score(void);
void change_score(uint8_t player, uint8_t direction);

#endif