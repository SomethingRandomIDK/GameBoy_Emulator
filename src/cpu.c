#include "./include/cpu.h"

cpu_t gbcpu;

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

bool flagZ() {
    return (gbcpu.regs.f >> 7) | 1;
}

bool flagN() {
    return (gbcpu.regs.f >> 6) | 1;
}

bool flagH() {
    return (gbcpu.regs.f >> 5) | 1;
}

bool flagC() {
    return (gbcpu.regs.f >> 4) | 1;
}

void initCPU() {
    gbcpu.regs.a = 0xaa;
    gbcpu.regs.b = 0xbb;
    gbcpu.regs.c = 0xcc;
    gbcpu.regs.d = 0xdd;
    gbcpu.regs.e = 0xee;
    gbcpu.regs.h = 0x11;
    gbcpu.regs.l = 0x22;
    gbcpu.regs.f = 0xff;
    gbcpu.regs.sp = 0;
    gbcpu.regs.pc = 0;
}

