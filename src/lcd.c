#include <stdbool.h>

#include "./include/lcd.h"
#include "./include/bus.h"

#define LCDC 0x0
#define STAT 0x1
#define SCRLX 0x2
#define SCRLY 0x3
#define LY 0x4
#define LYC 0x5
#define DMA 0x6
#define BGP 0x7
#define OBP0 0x8
#define OBP1 0x9
#define WY 0xa
#define WX 0xb

static bool dmaTransfer = false;
static uint8_t upperAddr;
static uint8_t lowerAddr;
static uint32_t dmaClock = 0;
static uint32_t lcdClock = 0;

static uint8_t lcdRegs[] = {
    0x91, 0x81, 0x00, 0x00, 0x91, 0x00, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00
};

void incLCDTimer(uint32_t cycles) {
    dmaClock += cycles;
    lcdClock += cycles;
    while (dmaClock > 3 && dmaTransfer) {
        busWrite8((0xfe00 | lowerAddr), busRead8((upperAddr << 8) | lowerAddr));
        lowerAddr++;
        dmaClock -= 4;
        dmaTransfer = lowerAddr < 0xa0;
    }

    switch (lcdRegs[STAT] & 0x3) {
	case 0:
	    break;
	case 1:
	    break;
	case 2:
	    break;
	case 3:
	    break;
    }
}

uint8_t readLCD(uint16_t addr) {
    return lcdRegs[addr - 0xff40];
}

void writeLCD(uint16_t addr, uint8_t val) {
    if (addr == (DMA + 0xff40)) {
        dmaTransfer = true;
        upperAddr = val;
        lowerAddr = 0;
        dmaClock = 0;
    }
    lcdRegs[addr - 0xff40] = val;
}

