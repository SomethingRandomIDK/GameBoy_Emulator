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

static void jp(gb_t *cpu) {
    char msg[64];
    sprintf(msg, "Inst: JP  InstCode: 0xc3 PC: %x", cpu->regs.pc);
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc = addr;
    logMessage(msg, TRACE);
}

static inst instructions[0x100] = {
    [0x00] = &nop,
    [0xc3] = &jp
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

