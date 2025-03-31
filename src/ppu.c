#include "./include/ppu.h"
#include "./include/lcd.h"

static uint8_t vram[0x2000];
static uint8_t oam[0xa0];
static uint8_t screen[144][160];

void drawLine() {
    return;
}

void drawFrame() {
    return;
}

uint8_t ppuVramRead(uint16_t addr) {
    if ((readLCD(0xff41) & 0x3) == 3) {
	return 0xff;
    }
    return vram[addr - 0x8000];
}

void ppuVramWrite(uint16_t addr, uint8_t val) {
    if ((readLCD(0xff41) & 0x3) == 3) {
	return;
    }
    vram[addr - 0x8000] = val;
}

uint8_t ppuOamRead(uint16_t addr) {
    if (((readLCD(0xff41) & 0x3) > 1) || dmaTransfering()) {
	return 0xff;
    }
    return oam[addr - 0xfe00];
}

void ppuOamWrite(uint16_t addr, uint8_t val) {
    if (((readLCD(0xff41) & 0x3) > 1) || dmaTransfering()) {
	return;
    }
    oam[addr - 0xfe00] = val;
}

void dmaWrite(uint16_t addr, uint8_t val) {
    oam[addr - 0xfe00] = val;
}

