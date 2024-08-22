#include "./include/cpu.h"

cpu_t gbCPU;

uint16_t regAF() {
    uint16_t ret;
    ret = (gbCPU.regs.a << 8) | gbCPU.regs.f;
    return ret;
}

uint16_t regBC() {
    uint16_t ret;
    ret = (gbCPU.regs.b << 8) | gbCPU.regs.c;
    return ret;
}

uint16_t regDE() {
    uint16_t ret;
    ret = (gbCPU.regs.d << 8) | gbCPU.regs.e;
    return ret;
}

uint16_t regHL() {
    uint16_t ret;
    ret = (gbCPU.regs.h << 8) | gbCPU.regs.l;
    return ret;
}

void initCPU() {
    gbCPU.regs.a = 0xaa;
    gbCPU.regs.b = 0xbb;
    gbCPU.regs.c = 0xcc;
    gbCPU.regs.d = 0xdd;
    gbCPU.regs.e = 0xee;
    gbCPU.regs.h = 0x11;
    gbCPU.regs.l = 0x22;
    gbCPU.regs.f = 0xff;
    gbCPU.regs.sp = 0;
    gbCPU.regs.pc = 0;
}

