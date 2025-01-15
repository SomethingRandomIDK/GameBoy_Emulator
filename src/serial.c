#include <stdbool.h>

#include "./include/serial.h"

static uint8_t transfer = 0;
static uint32_t serialCycles = 0;
static uint8_t buff = 0;
static uint8_t sb = 0;
static uint8_t sc = 0;

void incSerialTimer(uint32_t cycles) {
    if (transfer && (sc & 0x1)) {
        serialCycles += cycles;
        if ((serialCycles >> 9) & 1) return;
    }
}

