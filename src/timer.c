#include "./include/timer.h"
#include "./include/cart.h"
#include "./include/interrupt.h"
#include "./include/serial.h"

#define CLOCK 4194304

// Can't name something div because of the function in the stdlib.h
static uint16_t divReg = 0xab00;
static uint8_t tima = 0;
static uint8_t tma = 0;
static uint8_t tac = 0;
static uint32_t mbc3Cycles = 0;

void incTimer(uint32_t cycles) {
    mbc3Cycles += cycles;

    if(mbc3Cycles >= CLOCK) {
        updateRtcRegs();
        mbc3Cycles -= CLOCK;
    }

    uint16_t prevDiv = divReg;
    divReg += cycles;

    bool checkIncTIMA = false;
    switch(tac & 0x3) {
        case 0x0:
            checkIncTIMA = ((prevDiv >> 10) & 1) ^ ((divReg >> 9) & 1);
            break;
        case 0x1:
            checkIncTIMA = ((prevDiv >> 4) & 1) ^ ((divReg >> 3) & 1);
            break;
        case 0x2:
            checkIncTIMA = ((prevDiv >> 6) & 1) ^ ((divReg >> 5) & 1);
            break;
        case 0x3:
            checkIncTIMA = ((prevDiv >> 8) & 1) ^ ((divReg >> 7) & 1);
            break;
    }

    if ((tac & 0x04) && checkIncTIMA) {
        tima++;
        if (!(tima)) {
            tima = tma;
            raiseInterrupt(TIMER);
        }
    }
}

void incAllTimers(uint32_t cycles) {
    incTimer(cycles);
    incSerialTimer(cycles);
}

uint8_t readTimer(uint16_t addr) {
    switch(addr) {
        case 0xff04:
            return divReg >> 8;
        case 0xff05:
            return tima;
        case 0xff06:
            return tma;
        case 0xff07:
            return tac;
    }
    // Should never reach here
    return 0xff;
}

void writeTimer(uint16_t addr, uint8_t val) {
    switch(addr) {
        case 0xff04:
            divReg = 0;
            break;
        case 0xff05:
            tima = val;
            break;
        case 0xff06:
            tma = val;
            break;
        case 0xff07:
            tac = val;
            break;
    }
}

