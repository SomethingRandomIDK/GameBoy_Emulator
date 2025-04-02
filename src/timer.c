#include "./include/timer.h"
#include "./include/cart.h"
#include "./include/interrupt.h"
#include "./include/serial.h"
#include "./include/lcd.h"

#define CLOCK 4194304

// Can't name something div because of the function in the stdlib.h
static uint16_t divReg = 0xabcc;
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

    if (tac & 0x04) {
        uint16_t timaCounter = 0;
        switch(tac & 0x3) {
            case 0x0:
                timaCounter = ((divReg >> 10)) - ((prevDiv >> 10));
                break;
            case 0x1:
                timaCounter = ((divReg >> 4)) - ((prevDiv >> 4));
                break;
            case 0x2:
                timaCounter = ((divReg >> 6)) - ((prevDiv >> 6));
                break;
            case 0x3:
                timaCounter = ((divReg >> 8)) - ((prevDiv >> 8));
                break;
        }

        for (uint16_t i = 0; i < timaCounter; i++) {
            tima++;
            if (!(tima)) {
                raiseInterrupt(TIMER);
                tima = tma;
            }
        }
    }
}

void incAllTimers(uint32_t cycles) {
    incTimer(cycles);
    incSerialTimer(cycles);
    incLCDTimer(cycles);
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

