#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

void incAllTimers(uint32_t cycles);

void incTimer(uint32_t cycles);

uint8_t readTimer(uint16_t addr);

void writeTimer(uint16_t addr, uint8_t val);

#endif

