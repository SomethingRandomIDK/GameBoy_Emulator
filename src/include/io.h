#ifndef __IO_C__
#define __IO_C__

#include <stdint.h>

uint8_t ioRead(uint16_t addr);

void ioWrite(uint16_t addr, uint8_t val);

#endif

