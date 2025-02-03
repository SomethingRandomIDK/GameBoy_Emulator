#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>

uint8_t readLCD(uint16_t addr);
void writeLCD(uint16_t addr, uint8_t val);

#endif

