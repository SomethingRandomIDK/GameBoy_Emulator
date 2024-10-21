#include <stdio.h>
#include <stdlib.h>

#include "./logging/log.h"
#include "./include/interpreter.h"
#include "./include/bus.h"

typedef void (*inst)(gb_t *);

static void nop(gb_t *cpu) {
    cpu->regs.pc ++;
    logMessage("Inst: NOP  InstCode: 0x00", TRACE);
}

static inst instructions[0x100] = {
    [0x00] = &nop
};

void runInst(gb_t *cpu) {
    uint8_t curInst = busRead8(cpu->regs.pc);
    if (instructions[curInst]) {
        instructions[curInst](cpu);
    } else {
        char msg[64];
        sprintf(msg, "Instruction not recognized INST: %x", curInst);
        logMessage(msg, ERROR);
        exit(-1);
    }
}

