#include <stdio.h>

#include "./include/cpu.h"

cpu_t gbcpu;

// This will do everything that the bootrom is supposed to do on start up
static void bootRom() {
    gbcpu.regs.sp = 0xfffe;
    gbcpu.regs.a = 0x00;
    setHL(0x9fff);
    while (regHL() > 0x7fff) {
	break;
	// bus write the entire VRAM to zero
    }
    printf("boot rom not fully implemented yet\n");
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

void initCPU() {
    bootRom();
    gbcpu.regs.a = 0xaa;
    gbcpu.regs.b = 0xbb;
    gbcpu.regs.c = 0xcc;
    gbcpu.regs.d = 0xdd;
    gbcpu.regs.e = 0xee;
    gbcpu.regs.h = 0x11;
    gbcpu.regs.l = 0x22;
    gbcpu.regs.f = 0x00;
    gbcpu.regs.sp = 0;
    gbcpu.regs.pc = 0;
}

