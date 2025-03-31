#ifndef __LCD_H__
#define __LCD_H__

#include <stdint.h>
#include <stdbool.h>

bool dmaTransfering();

uint8_t readLCD(uint16_t addr);
void writeLCD(uint16_t addr, uint8_t val);

#endif

