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
        buff[buffIdx] = (char) sb;
        buffIdx++;
        sb = 0xff;
        sc &= 0x7f;
        raiseInterrupt(SERIAL);
        printf("%s\n", buff);
        // serialCycles += cycles;
        // if ((serialCycles >> 9) & 1) {
        //     printf("%c", (char)sb);
        //     sb = 0xff;
        //     sc &= 0x7f;
        //     serialCycles = 0;
        //     raiseInterrupt(SERIAL);
        // }
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

