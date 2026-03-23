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

#include "./include/timer.h"
#include "./include/cart.h"
#include "./include/interrupt.h"
#include "./include/serial.h"
#include "./include/lcd.h"
#include "./include/gui.h"
#include "./include/apu.h"

#define CLOCK 4194304

// Can't name something div because of the function in the stdlib.h
static uint16_t divReg = 0xabcc;
static uint8_t tima = 0;
static uint8_t tma = 0;
static uint8_t tac = 0;
static uint32_t mbc3Cycles = 0;

void incTimer(uint32_t cycles) {
    mbc3Cycles += cycles;
    incApuTimer(cycles);

    if(mbc3Cycles >= CLOCK) {
        updateRtcRegs();
        mbc3Cycles -= CLOCK;
    }

    uint16_t prevDiv = divReg;
    divReg += cycles;

    if (tac & 0x04) {
        bool timaCounter = false;
        switch(tac & 0x3) {
            case 0x0:
                timaCounter = ((divReg >> 10)) > ((prevDiv >> 10));
                break;
            case 0x1:
                timaCounter = ((divReg >> 4)) > ((prevDiv >> 4));
                break;
            case 0x2:
                timaCounter = ((divReg >> 6)) > ((prevDiv >> 6));
                break;
            case 0x3:
                timaCounter = ((divReg >> 8)) > ((prevDiv >> 8));
                break;
        }

        if (timaCounter) {
            tima++;
            if (!(tima)) {
                raiseInterrupt(TIMER);
                tima = tma;
            }
        }
    }
}

void incAllTimers(uint32_t cycles) {
    incEventTimer(cycles);
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

