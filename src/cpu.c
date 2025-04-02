#include <stdio.h>

#include "./include/cpu.h"
#include "./include/gui.h"
#include "./include/interpreter.h"
#include "./include/interrupt.h"
#include "./include/timer.h"
#include "./logging/log.h"

gb_t gbcpu;

bool running = true;

void stopRunning() {
    running = false;
}

uint16_t regAF() {
    uint16_t ret;
    ret = (gbcpu.regs.a << 8) | gbcpu.regs.f;
    return ret;
}

uint16_t regBC() {
    uint16_t ret;
    ret = (gbcpu.regs.b << 8) | gbcpu.regs.c;
    return ret;
}

uint16_t regDE() {
    uint16_t ret;
    ret = (gbcpu.regs.d << 8) | gbcpu.regs.e;
    return ret;
}

uint16_t regHL() {
    uint16_t ret;
    ret = (gbcpu.regs.h << 8) | gbcpu.regs.l;
    return ret;
}

void setAF(uint16_t val) {
    gbcpu.regs.a = (val >> 8) & 0xff;
    gbcpu.regs.f = val & 0xff;
}

void setBC(uint16_t val) {
    gbcpu.regs.b = (val >> 8) & 0xff;
    gbcpu.regs.c = val & 0xff;
}

void setDE(uint16_t val) {
    gbcpu.regs.d = (val >> 8) & 0xff;
    gbcpu.regs.e = val & 0xff;
}

void setHL(uint16_t val) {
    gbcpu.regs.h = (val >> 8) & 0xff;
    gbcpu.regs.l = val & 0xff;
}

bool flagZ() {
    return (gbcpu.regs.f >> 7) & 1;
}

bool flagN() {
    return (gbcpu.regs.f >> 6) & 1;
}

bool flagH() {
    return (gbcpu.regs.f >> 5) & 1;
}

bool flagC() {
    return (gbcpu.regs.f >> 4) & 1;
}

void setZ(bool newZ) {
    if (newZ)
        gbcpu.regs.f |= 0x80;
    else
        gbcpu.regs.f &= 0x7f;
}

void setN(bool newN) {
    if (newN)
        gbcpu.regs.f |= 0x40;
    else
        gbcpu.regs.f &= 0xbf;
}

void setH(bool newH) {
    if (newH)
        gbcpu.regs.f |= 0x20;
    else
        gbcpu.regs.f &= 0xdf;
}

void setC(bool newC) {
    if (newC)
        gbcpu.regs.f |= 0x10;
    else
        gbcpu.regs.f &= 0xef;
}

uint8_t wramRead(uint16_t addr) {
    // From where it is used the addr variable should never exceed 0xe000
    return gbcpu.wram[addr - 0xc000];
}

void wramWrite(uint16_t addr, uint8_t val) {
    gbcpu.wram[addr - 0xc000] = val;
}

uint8_t hramRead(uint16_t addr) {
    return gbcpu.hram[addr - 0xff80];
}

void hramWrite(uint16_t addr, uint8_t val){ 
    gbcpu.hram[addr - 0xff80] = val;
}

void initCPU() {
    gbcpu.regs.a = 0x01;
    setBC(0x0013);
    setDE(0x00d8);
    setHL(0x014d);
    gbcpu.regs.f = 0xb0;
    gbcpu.regs.sp = 0xfffe;
    gbcpu.regs.pc = 0x100;
    gbcpu.interrupts = false;
}

void startCPU() {
    initLogger(NULL, TRACE);
    initGUI();

    while(running) {
        if(gbcpu.halted || gbcpu.stopped) {
            incAllTimers(4);
        } else {
            step(&gbcpu);
        }

        handleInterrupt(&gbcpu);
    }

    closeGUI();
}

