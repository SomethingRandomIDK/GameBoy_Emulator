#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint8_t f;
    uint16_t sp;
    uint16_t pc;
} registers_t;

typedef struct {
    registers_t regs;
} cpu_t;

uint16_t regAF ();
uint16_t regBC ();
uint16_t regDE ();
uint16_t regHL ();

void initCPU();

#endif

