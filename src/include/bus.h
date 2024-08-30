#ifndef _BUS_H_
#define _BUS_H_

#include <stdint.h>

uint8_t busRead8(uint16_t addr);
void busWrite8(uint16_t addr, uint8_t val);

uint16_t busRead16(uint16_t addr);
void busWrite16(uint16_t addr, uint16_t val);

#endif

