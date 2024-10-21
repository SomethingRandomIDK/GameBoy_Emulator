#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>
#include <stdbool.h>

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
    uint8_t wram[0x2000];
    uint8_t hram[0x7f];
} gb_t;

uint16_t regAF();
uint16_t regBC();
uint16_t regDE();
uint16_t regHL();

void setAF(uint16_t val);
void setBC(uint16_t val);
void setDE(uint16_t val);
void setHL(uint16_t val);

bool flagZ();
bool flagN();
bool flagH();
bool flagC();

uint8_t wramRead(uint16_t addr);
void wramWrite(uint16_t addr, uint8_t val);

uint8_t hramRead(uint16_t addr);
void hramWrite(uint16_t addr, uint8_t val);

void initCPU(void);

void startCPU(void);

#endif

