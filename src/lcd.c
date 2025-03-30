#include <stdbool.h>

#include "./include/lcd.h"
#include "./include/interrupt.h"
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

static void checkLy() {
    if (lcdRegs[LY] == lcdRegs[LYC]) {
	lcdRegs[STAT] |= 0x4;
	if (lcdRegs[STAT] & 0x40) {
	    raiseInterrupt(LCD);
	}
    } else {
	lcdRegs[STAT] &= ~(0x4);
    }
}

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
	case 0: // Horizontal Blank
	    if (lcdClock >= 456) {
		lcdClock -= 456;
		lcdRegs[LY]++;
		checkLy();
		if (lcdRegs[LY] >= 144) {
		    lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x1;

		    raiseInterrupt(VBLANK);

		    if (lcdRegs[STAT] & 0x10) {
			raiseInterrupt(LCD);
		    }

		    // TODO Draw Screen Here

		} else {
		    lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x2;

		    if (lcdRegs[STAT] & 0x20) {
			raiseInterrupt(LCD);
		    }
		}
	    }
	    break;
	case 1: // Vertical Blank
	    if (lcdClock >= 456) {
		lcdClock -= 456;
		bool newFrame = false;
		lcdRegs[LY]++;
		if (lcdRegs[LY] >= 154) {
		    lcdRegs[LY] = 0;
		    newFrame = true;
		    checkLy();
		    lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x2;

		    if (lcdRegs[STAT] & 0x20) {
			raiseInterrupt(LCD);
		    }
		}
	    }
	    break;
	case 2: // OAM scan
	    if (lcdClock >= 80) {
		lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x3;
	    }
	    break;
	case 3: // Drawing Pixels
	    if (lcdClock >= 252) {
		lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x0;
		if (lcdRegs[STAT] & 0x08) {
		    raiseInterrupt(LCD);
		}
	    }
	    break;
    }
}

uint8_t readLCD(uint16_t addr) {
    return lcdRegs[addr - 0xff40];
}

void writeLCD(uint16_t addr, uint8_t val) {
    /* if (addr == (DMA + 0xff40)) { */
    /*     dmaTransfer = true; */
    /*     upperAddr = val; */
    /*     lowerAddr = 0; */
    /*     dmaClock = 0; */
    /* } */
    /* lcdRegs[addr - 0xff40] = val; */
    switch(addr - 0xff40) {
	case DMA:
	    dmaTransfer = true;
	    upperAddr = val;
	    lowerAddr = 0;
	    dmaClock = 0;
	    break;
	case STAT:
	    if (lcdRegs[LCDC] & 0x80) {
		if (((val & 0x08) ^ (lcdRegs[STAT] & 0x08)) && (lcdRegs[STAT] & 0x3) == 0) {
		    raiseInterrupt(LCD);
		}
		if (((val & 0x10) ^ (lcdRegs[STAT] & 0x10)) && (lcdRegs[STAT] & 0x3) == 1) {
		    raiseInterrupt(LCD);
		}
		if (((val & 0x20) ^ (lcdRegs[STAT] & 0x20)) && (lcdRegs[STAT] & 0x3) == 2) {
		    raiseInterrupt(LCD);
		}
		if ((val & 0x40) ^ (lcdRegs[STAT] & 0x40)) {
		    checkLy();
		}
	    }

	    lcdRegs[STAT] = (val & 0x78) | (lcdRegs[STAT] & 0x7);
	    break;
	case LYC:
	    lcdRegs[LYC] = val;
	    if (lcdRegs[LCDC] & 0x80) {
		checkLy();
	    }
	    break;
	case LY:
	    break;
	default:
	    lcdRegs[addr - 0xff40] = val;
	    break;
    } 
}

