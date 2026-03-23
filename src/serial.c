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

#include <stdio.h>
#include <stdbool.h>

#include "./include/serial.h"
#include "./include/interrupt.h"

static uint32_t serialCycles = 0;
static char buff[1024];
static uint16_t buffIdx = 0;
static uint8_t sb = 0;
static uint8_t sc = 0;

void incSerialTimer(uint32_t cycles) {
    if ((sc & 0x81) == 0x81) {
        // buff[buffIdx] = (char) sb;
        // buffIdx++;
        // sb = 0xff;
        // sc &= 0x7f;
        // raiseInterrupt(SERIAL);
        // printf("%s\n", buff);
        serialCycles += cycles;
        if ((serialCycles >> 9) & 1) {
            printf("%c", (char)sb);
            fflush(stdout);
            sb = 0xff;
            sc &= 0x7f;
            serialCycles = 0;
            raiseInterrupt(SERIAL);
        }
    }
}

uint8_t readSB() {
    return sb;
}

uint8_t readSC() {
    return sc;
}

void writeSB(uint8_t val) {
    sb = val;
}

void writeSC(uint8_t val) {
    sc = val;
    serialCycles = 0;
}

