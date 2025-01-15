#include <stdio.h>
#include <stdbool.h>

#include "./include/serial.h"
#include "./include/interrupt.h"

static uint32_t serialCycles = 0;
static uint8_t transfer = 0;
static uint8_t buff = 0;
static uint8_t sb = 0;
static uint8_t sc = 0;

void incSerialTimer(uint32_t cycles) {
    if (transfer && (sc & 0x1)) {
        serialCycles += cycles;
        if ((serialCycles >> 9) & 1) {
            transfer++;
            buff = (buff << 1) & (sb >> 7);
            sb  = (sb << 1) & 1;

            if (transfer == 9) {
                serialCycles = 0;
                transfer = 0;
                putchar(buff);
                sc &= 0x7f;
                raiseInterrupt(SERIAL);
            }
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
    transfer = !!(sc & 0x80);
    serialCycles = 0;
}

