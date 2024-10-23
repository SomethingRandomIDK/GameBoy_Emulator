#include <stdio.h>
#include <stdlib.h>

#include "./logging/log.h"
#include "./include/interpreter.h"
#include "./include/bus.h"

typedef void (*inst)(gb_t *);

static void nop(gb_t *cpu) {
    char msg[64];
    sprintf(msg, "Inst: NOP  InstCode: 0x00 PC: %x", cpu->regs.pc); 
    cpu->regs.pc ++;
    logMessage(msg, TRACE);
}

// XOR

static void xor_a(gb_t *cpu) {
    cpu->regs.a ^= cpu->regs.a;
    cpu->regs.f |= 0x80;
    cpu->regs.f &= 0x8f;
    char msg[64];
    sprintf(msg, "Inst: XOR A  InstCode: 0xaf PC: %x A: %x F: %x", cpu->regs.pc, cpu->regs.a, cpu->regs.f);
    cpu->regs.pc++;
    logMessage(msg, TRACE);
}

// INTERUPTS

static void di(gb_t *cpu) {
    char msg[64];
    sprintf(msg, "Inst: DI InstCode: 0xf3 PC: %x", cpu->regs.pc);
    cpu->interrupts = false;
    cpu->regs.pc++;
    logMessage(msg, TRACE);
}

// JUMPS

static void jp(gb_t *cpu) {
    char msg[64];
    sprintf(msg, "Inst: JP  InstCode: 0xc3 PC: %x", cpu->regs.pc);
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc = addr;
    logMessage(msg, TRACE);
}

static void jp_nz(gb_t *cpu) {
    char msg[64];
    sprintf(msg, "Inst: JP NZ  InstCode: 0xc2 PC: %x", cpu->regs.pc);
    if ((cpu->regs.f & 0x80) == 0) {
        uint16_t addr = busRead16(++cpu->regs.pc);
        cpu->regs.pc = addr;
    } else {
        cpu->regs.pc += 3;
    }
    logMessage(msg, TRACE);
}

static inst instructions[0x100] = {
    [0x00] = &nop,
    [0xaf] = &xor_a,
    [0xc2] = &jp_nz,
    [0xc3] = &jp,
    [0xf3] = &di
};

void runInst(gb_t *cpu) {
    uint8_t curInst = busRead8(cpu->regs.pc);
    if (instructions[curInst]) {
        instructions[curInst](cpu);
    } else {
        char msg[64];
        sprintf(msg, "Instruction not recognized INST: %x PC: %x", curInst, cpu->regs.pc);
        logMessage(msg, ERROR);
        exit(-1);
    }
}

