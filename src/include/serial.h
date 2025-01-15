#ifndef __SERIAL_H__
#define __SERIAL_H__

#include <stdint.h>

void incSerialTimer(uint32_t cycles);

uint8_t readSB();

uint8_t readSC();

void writeSB(uint8_t val);

void writeSC(uint8_t val);

#endif

