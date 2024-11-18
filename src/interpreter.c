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
    [0x30] = "JR NC n",
    [0x31] = "LD SP nn",
    [0x32] = "LD (HL-) A",
    [0x33] = "INC SP",
    [0x34] = "INC (HL)",
    [0x35] = "DEC (HL)",
    [0x36] = "LD (HL) n",
    [0x37] = "SCF",
    [0x38] = "JR C n",
    [0x39] = "ADD HL SP",
    [0x3a] = "LD A (HL-)",
    [0x3b] = "DEC SP",
    [0x3c] = "INC A",
    [0x3d] = "DEC A",
    [0x3e] = "LD A n",
    [0x3f] = "CCF",

    // 0x40 - 0x4f
    [0x40] = "LD B B",
    [0x41] = "LD B C",
    [0x42] = "LD B D",
    [0x43] = "LD B E",
    [0x44] = "LD B H",
    [0x45] = "LD B L",
    [0x46] = "LD B (HL)",
    [0x47] = "LD B A",
    [0x48] = "LD C B",
    [0x49] = "LD C C",
    [0x4a] = "LD C D",
    [0x4b] = "LD C E",
    [0x4c] = "LD C H",
    [0x4d] = "LD C L",
    [0x4e] = "LD C (HL)",
    [0x4f] = "LD C A",

    // 0x50 - 0x5f
    [0x50] = "LD D B",
    [0x51] = "LD D C",
    [0x52] = "LD D D",
    [0x53] = "LD D E",
    [0x54] = "LD D H",
    [0x55] = "LD D L",
    [0x56] = "LD D (HL)",
    [0x57] = "LD D A",
    [0x58] = "LD E B",
    [0x59] = "LD E C",
    [0x5a] = "LD E D",
    [0x5b] = "LD E E",
    [0x5c] = "LD E H",
    [0x5d] = "LD E L",
    [0x5e] = "LD E (HL)",
    [0x5f] = "LD E A",

    // 0x60 - 0x6f
    [0x60] = "LD H B",
    [0x61] = "LD H C",
    [0x62] = "LD H D",
    [0x63] = "LD H E",
    [0x64] = "LD H H",
    [0x65] = "LD H L",
    [0x66] = "LD H (HL)",
    [0x67] = "LD H A",
    [0x68] = "LD L B",
    [0x69] = "LD L C",
    [0x6a] = "LD L D",
    [0x6b] = "LD L E",
    [0x6c] = "LD L H",
    [0x6d] = "LD L L",
    [0x6e] = "LD L (HL)",
    [0x6f] = "LD L A",

    // 0x70 - 0x7f
    [0x70] = "LD (HL) B",
    [0x71] = "LD (HL) C",
    [0x72] = "LD (HL) D",
    [0x73] = "LD (HL) E",
    [0x74] = "LD (HL) H",
    [0x75] = "LD (HL) L",
    [0x76] = "HALT",
    [0x77] = "LD (HL) A",
    [0x78] = "LD A B",
    [0x79] = "LD A C",
    [0x7a] = "LD A D",
    [0x7b] = "LD A E",
    [0x7c] = "LD A H",
    [0x7d] = "LD A L",
    [0x7e] = "LD A (HL)",
    [0x7f] = "LD A A",

    // 0x80 - 0x8f
    [0x80] = "ADD A B",
    [0x81] = "ADD A C",
    [0x82] = "ADD A D",
    [0x83] = "ADD A E",
    [0x84] = "ADD A H",
    [0x85] = "ADD A L",
    [0x86] = "ADD A (HL)",
    [0x87] = "ADD A A",
    [0x88] = "ADC A B",
    [0x89] = "ADC A C",
    [0x8a] = "ADC A D",
    [0x8b] = "ADC A E",
    [0x8c] = "ADC A H",
    [0x8d] = "ADC A L",
    [0x8e] = "ADC A (HL)",
    [0x8f] = "ADC A A",

    // 0x90 - 0x9f
    [0x90] = "SUB B",
    [0x91] = "SUB C",
    [0x92] = "SUB D",
    [0x93] = "SUB E",
    [0x94] = "SUB H",
    [0x95] = "SUB L",
    [0x96] = "SUB (HL)",
    [0x97] = "SUB A",
    [0x98] = "SBC A B",
    [0x99] = "SBC A C",
    [0x9a] = "SBC A D",
    [0x9b] = "SBC A E",
    [0x9c] = "SBC A H",
    [0x9d] = "SBC A L",
    [0x9e] = "SBC A (HL)",
    [0x9f] = "SBC A A",

    // 0xa0 - 0xaf
    [0xa0] = "AND B",
    [0xa1] = "AND C",
    [0xa2] = "AND D",
    [0xa3] = "AND E",
    [0xa4] = "AND H",
    [0xa5] = "AND L",
    [0xa6] = "AND (HL)",
    [0xa7] = "AND A",
    [0xa8] = "XOR B",
    [0xa9] = "XOR C",
    [0xaa] = "XOR D",
    [0xab] = "XOR E",
    [0xac] = "XOR H",
    [0xad] = "XOR L",
    [0xae] = "XOR (HL)",
    [0xaf] = "XOR A",

    // 0xb0 - 0xbf
    [0xb0] = "OR B",
    [0xb1] = "OR C",
    [0xb2] = "OR D",
    [0xb3] = "OR E",
    [0xb4] = "OR H",
    [0xb5] = "OR L",
    [0xb6] = "OR (HL)",
    [0xb7] = "OR A",
    [0xb8] = "CP B",
    [0xb9] = "CP C",
    [0xba] = "CP D",
    [0xbb] = "CP E",
    [0xbc] = "CP H",
    [0xbd] = "CP L",
    [0xbe] = "CP (HL)",
    [0xbf] = "CP A",

    // 0xc0 - 0xcf
    [0xc0] = "RET NZ",
    [0xc1] = "POP BC",
    [0xc2] = "JP NZ nn",
    [0xc3] = "JP nn",
    [0xc4] = "CALL NZ nn",
    [0xc5] = "PUSH BC",
    [0xc6] = "ADD A n",
    [0xc7] = "RST 00H",
    [0xc8] = "RET Z",
    [0xc9] = "RET",
    [0xca] = "JP Z nn",
    [0xcb] = "PREFIX CB",
    [0xcc] = "CALL Z nn",
    [0xcd] = "CALL nn",
    [0xce] = "ADC A n",
    [0xcf] = "RST 08H",

    // 0xd0 - 0xdf
    [0xd0] = "RET NC",
    [0xd1] = "POP DE",
    [0xd2] = "JP NC nn",
    [0xd3] = NULL,
    [0xd4] = "CALL NC nn",
    [0xd5] = "PUSH DE",
    [0xd6] = "SUB n",
    [0xd7] = "RST 10H",
    [0xd8] = "RET C",
    [0xd9] = "RETI",
    [0xda] = "JP C nn",
    [0xdb] = NULL,
    [0xdc] = "CALL C nn",
    [0xdd] = NULL,
    [0xde] = "SBC A n",
    [0xdf] = "RST 18H",

    // 0xe0 - 0xef
    [0xe0] = "LDH (n) A",
    [0xe1] = "POP HL",
    [0xe2] = "LD (C) A",
    [0xe3] = NULL,
    [0xe4] = NULL,
    [0xe5] = "PUSH HL",
    [0xe6] = "AND n",
    [0xe7] = "RST 20H",
    [0xe8] = "ADD SP n",
    [0xe9] = "JP (HL)",
    [0xea] = "LD (nn) A",
    [0xeb] = NULL,
    [0xec] = NULL,
    [0xed] = NULL,
    [0xee] = "XOR n",
    [0xef] = "RST 28H",

    // 0xf0 - 0xff
    [0xf0] = "LDH A (n)",
    [0xf1] = "POP HL",
    [0xf2] = "LD A (C)",
    [0xf3] = "DI",
    [0xf4] = NULL,
    [0xf5] = "PUSH AF",
    [0xf6] = "OR n",
    [0xf7] = "RST 30H",
    [0xf8] = "LD HL SP+n",
    [0xf9] = "LD SP HL",
    [0xfa] = "LD A (nn)",
    [0xfb] = "EI",
    [0xfc] = NULL,
    [0xfd] = NULL,
    [0xfe] = "CP n",
    [0xff] = "RST 38H",
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

