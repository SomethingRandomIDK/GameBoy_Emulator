#ifndef __APU_H__
#define __APU_H__

#include <stdint.h>

void incApuTimer(uint32_t cycles);

uint8_t readSound(uint16_t addr);
void writeSound(uint16_t addr, uint8_t val);

#endif

