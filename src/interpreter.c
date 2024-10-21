#include <stdlib.h>

#include "./include/interpreter.h"
#include "./include/bus.h"

typedef void (*inst)(gb_t *);

static void nop(gb_t *cpu) {
    cpu->regs.pc ++;
}

static inst instructions[] = {
    [0x00] = &nop
};

void runInst(gb_t *cpu) {
    uint8_t curInst = busRead8(cpu->regs.pc);
    if (instructions[curInst]) {
        instructions[curInst](cpu);
    } else {
        exit(-1);
    }
}

