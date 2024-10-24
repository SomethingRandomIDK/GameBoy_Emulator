#include <stdio.h>
#include <stdlib.h>

#include "./logging/log.h"
#include "./include/interpreter.h"
#include "./include/bus.h"
#include "./include/cpu.h"

typedef void (*inst)(gb_t *);

// NOP

static void nop(gb_t *cpu) {
    cpu->regs.pc ++;
}

// 8-BIT LD

static void ld_nn_a(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc += 2;
    busWrite8(addr, cpu->regs.a);
}

// 16-BIT LD

static void ld_sp_nn(gb_t* cpu) {
    cpu->regs.sp = busRead16(++cpu->regs.pc);
    cpu->regs.pc += 2;
}

// XOR

static void xor_a(gb_t *cpu) {
    cpu->regs.a ^= cpu->regs.a;
    cpu->regs.f |= 0x80;
    cpu->regs.f &= 0x8f;
    cpu->regs.pc++;
}

// INTERUPTS

static void di(gb_t *cpu) {
    cpu->interrupts = false;
    cpu->regs.pc++;
}

// JUMPS

static void jp_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc = addr;
}

static void jp_nz_nn(gb_t *cpu) {
    if (!flagZ()) {
        uint16_t addr = busRead16(++cpu->regs.pc);
        cpu->regs.pc = addr;
    } else {
        cpu->regs.pc += 3;
    }
}

static void jr_n(gb_t *cpu) {
    int8_t jmpDiff = (int8_t)busRead8(++cpu->regs.pc);
    cpu->regs.pc++;
    cpu->regs.pc += jmpDiff;
}

static inst instructions[0x100] = {
    [0x00] = &nop,
    [0x18] = &jr_n,
    [0x31] = &ld_sp_nn,
    [0xaf] = &xor_a,
    [0xc2] = &jp_nz_nn,
    [0xc3] = &jp_nn,
    [0xea] = &ld_nn_a,
    [0xf3] = &di
};

static char *instNames[0x100] = {
    [0x00] = "NOP",
    [0x18] = "JR n",
    [0x31] = "LD SP nn",
    [0xaf] = "XOR A",
    [0xc2] = "JP NZ nn",
    [0xc3] = "JP nn",
    [0xea] = "LD nn A",
    [0xf3] = "DI"
};

void runInst(gb_t *cpu) {
    uint8_t opcode = busRead8(cpu->regs.pc);
    if (instructions[opcode]) {
        if (instNames[opcode]) {
            char msg[128];
            sprintf(msg, "INST: %s OPCODE: %02x PC: %04x SP: %04x A: %02x BC: %04x DE: %04x HL: %04x F: %02x",
                    instNames[opcode], opcode, cpu->regs.pc, cpu->regs.sp,
                    cpu->regs.a, regBC(), regDE(), regHL(), cpu->regs.f);
            logMessage(msg, TRACE);
        } else
            logMessage("Instruction Information Not Found", WARNING);

        instructions[opcode](cpu);
    } else {
        char msg[64];
        sprintf(msg, "Instruction not recognized INST: %x PC: %x", opcode, cpu->regs.pc);
        logMessage(msg, ERROR);
        exit(-1);
    }
}

