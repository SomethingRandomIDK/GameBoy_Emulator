#include "./include/io.h"
#include "./include/interrupt.h"
#include "./include/timer.h"

uint8_t ioRead(uint16_t addr) {
    switch(addr & 0xff) {
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            return readTimer(addr);
        case 0x0f:
            return interFlags;
        case 0xff:
            return interEnable;
        default:
            return 0;
    }
}

void ioWrite(uint16_t addr, uint8_t val) {
    switch(addr & 0xff) {
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            writeTimer(addr, val);
            break;
        case 0x0f:
            interFlags = (val | 0xe0);
            break;
        case 0xff:
            interEnable = val;
            break;
    }
}

