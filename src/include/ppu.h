#ifndef __PPU_H__
#define __PPU_H__

#include <stdint.h>

void resetWindowLine();

void drawLine();
void drawFrame();

uint8_t ppuVramRead(uint16_t addr);
void ppuVramWrite(uint16_t addr, uint8_t val);

uint8_t ppuOamRead(uint16_t addr);
void ppuOamWrite(uint16_t addr, uint8_t val);

void dmaWrite(uint16_t addr, uint8_t val);

#endif

