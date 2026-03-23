/*
    gbemu - A Cross-Platform Game Boy Emulator
    Copyright (C) 2024-2026  Mehul Tomar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "./cpu.h"

extern uint8_t interFlags;
extern uint8_t interEnable;

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

