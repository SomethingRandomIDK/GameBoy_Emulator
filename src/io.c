#include "./include/io.h"
#include "./include/interrupt.h"
#include "./include/timer.h"
#include "./include/serial.h"

uint8_t ioRead(uint16_t addr) {
    switch(addr) {
        case 0xff01:
            return readSB();
        case 0xff02:
            return readSC();
        case 0xff04:
        case 0xff05:
        case 0xff06:
        case 0xff07:
            return readTimer(addr);
        case 0xff0f:
            return interFlags;
        case 0xffff:
            return interEnable;
        default:
            return 0;
    }
}

void ioWrite(uint16_t addr, uint8_t val) {
    switch(addr) {
        case 0xff01:
            writeSB(val);
            break;
        case 0xff02:
            writeSC(val);
            break;
        case 0xff04:
        case 0xff05:
        case 0xff06:
        case 0xff07:
            writeTimer(addr, val);
            break;
        case 0xff0f:
            interFlags = (val | 0xe0);
            break;
        case 0xffff:
            interEnable = val;
            break;
    }
}

