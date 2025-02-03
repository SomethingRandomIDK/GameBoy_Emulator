#include <stdbool.h>

#include "./include/lcd.h"
#include "./include/bus.h"

#define LCDC 0xff40
#define STAT 0xff41
#define SCRLX 0xff42
#define SCRLY 0xff43
#define LY 0xff44
#define LYC 0xff45
#define DMA 0xff46
#define BGP 0xff47
#define OBP0 0xff48
#define OBP1 0xff49
#define WY 0xff4a
#define WX 0xff4b

static bool dmaTransfer = false;
static uint8_t upperAddr;
static uint8_t lowerAddr;
static uint32_t clock = 0;

static uint8_t lcdRegs[] = {
    0x91, 0x81, 0x00, 0x00, 0x91, 0x00, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00
};

void incLCDTimer(uint32_t cycles) {
    clock += cycles;
    while (clock > 4 && dmaTransfer) {
        busWrite8((0xfe00 & lowerAddr), busRead8((upperAddr < 8) & lowerAddr));
        lowerAddr++;
        clock -= 4;
        dmaTransfer = lowerAddr < 0xa0;
    }
}

uint8_t readLCD(uint16_t addr) {
    return lcdRegs[addr - 0xff40];
}

void writeLCD(uint16_t addr, uint8_t val) {
    if (addr == DMA) {
        dmaTransfer = true;
        upperAddr = val;
        lowerAddr = 0;
        clock = 0;
    }
    lcdRegs[addr - 0xff40] = val;
}

