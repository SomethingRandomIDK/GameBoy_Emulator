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

static void ld_reg_n(uint8_t *val, gb_t *cpu) {
    *val = busRead8(++cpu->regs.pc);
    cpu->regs.pc++;
}

static void ld_a_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.a, cpu);
}

static void ld_b_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.b, cpu);
}

static void ld_c_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.c, cpu);
}

static void ld_d_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.d, cpu);
}

static void ld_e_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.e, cpu);
}

static void ld_h_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.h, cpu);
}

static void ld_l_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.l, cpu);
}

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
    [0x06] = &ld_b_n,
    [0x0e] = &ld_c_n,
    [0x16] = &ld_d_n,
    [0x18] = &jr_n,
    [0x1e] = &ld_e_n,
    [0x26] = &ld_h_n,
    [0x2e] = &ld_l_n,
    [0x31] = &ld_sp_nn,
    [0x3e] = &ld_a_n,
    [0xaf] = &xor_a,
    [0xc2] = &jp_nz_nn,
    [0xc3] = &jp_nn,
    [0xea] = &ld_nn_a,
    [0xf3] = &di
};

static char *instNames[0x100] = {
    // 0x00 - 0x0f
    [0x00] = "NOP",
    [0x01] = "LD BC nn",
    [0x02] = "LD (BC) A",
    [0x03] = "INC BC",
    [0x04] = "INC B",
    [0x05] = "DEC B",
    [0x06] = "LD B n",
    [0x07] = "RLCA",
    [0x08] = "LD (nn) SP",
    [0x09] = "ADD HL BC",
    [0x0a] = "LD A (BC)",
    [0x0b] = "DEC BC",
    [0x0c] = "INC C",
    [0x0d] = "DEC C",
    [0x0e] = "LD C n",
    [0x0f] = "RRCA",

    // 0x10 - 0x1f
    [0x10] = "STOP 0",
    [0x11] = "LD DE nn",
    [0x12] = "LD (DE) A",
    [0x13] = "INC DE",
    [0x14] = "INC D",
    [0x15] = "DEC D",
    [0x16] = "LD D n",
    [0x17] = "RLA",
    [0x18] = "JR n",
    [0x19] = "ADD HL DE",
    [0x1a] = "LD A (DE)",
    [0x1b] = "DEC DE",
    [0x1c] = "INC E",
    [0x1d] = "DEC E",
    [0x1e] = "LD E n",
    [0x1f] = "RRA",

    // 0x20 - 0x2f
    [0x20] = "JR NZ n",
    [0x21] = "LD HL nn",
    [0x22] = "LD (HL+) A",
    [0x23] = "INC HL",
    [0x24] = "INC H",
    [0x25] = "DEC H",
    [0x26] = "LD H n",
    [0x27] = "DAA",
    [0x28] = "JR Z n",
    [0x29] = "ADD HL HL",
    [0x2a] = "LD A (HL+)",
    [0x2b] = "DEC HL",
    [0x2c] = "INC L",
    [0x2d] = "DEC L",
    [0x2e] = "LD L n",
    [0x2f] = "CPL",

    // 0x30 - 0x3f
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

