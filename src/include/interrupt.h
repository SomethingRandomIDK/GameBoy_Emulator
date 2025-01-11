#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "./cpu.h"

typedef enum {
    VBLANK = 0,
    LCD = 1,
    TIMER = 2,
    SERIAL = 3,
    JOYPAD = 4
} inter_t;

void raiseInterrupt(inter_t inter);

void handleInterrupt(gb_t *cpu);

#endif

