#include "./include/ppu.h"
#include "./include/gui.h"
#include "./include/lcd.h"
#include <stdint.h>

static uint8_t vram[0x2000];
static uint8_t oam[0xa0];
static uint8_t screen[144][160];

static uint8_t palette[4] = {0xff, 0xab, 0x55, 0x00};

enum tileData_t{
    TILE_BLOCK_1,
    TILE_BLOCK_2
};

static void drawBgLine(uint8_t lcdc, uint8_t line) {
    if (!(lcdc & 0x01)) {
	for (int i = 0; i < 160; i++) {
	    screen[line][i] = 0xff;
	}
	return;
    }

    uint8_t bgPaletteNum = readLCD(0xff47);
    uint8_t bgPalette[4] = {
	palette[bgPaletteNum & 0x3],
	palette[(bgPaletteNum >> 2) & 0x3],
	palette[(bgPaletteNum >> 4) & 0x3],
	palette[(bgPaletteNum >> 6) & 0x3]
    };

    // Actual map values are 0x9800 and 0x9c00, but we are going to be directly
    // accessing the vram
    uint16_t tileMapAddr = (lcdc & 0x8) ? 0x1800 : 0x1c00;
    enum tileData_t tileBlock = (lcdc & 0x10) ? TILE_BLOCK_1 : TILE_BLOCK_2;

    uint8_t yPix = readLCD(0xff43) + line;
    uint8_t xStart = readLCD(0xff42);
    uint8_t endLine = xStart + 160;
    // Sets xPix to the start of the tile
    uint8_t xPix = xStart & 0xf8;

    for (;xPix < endLine; xPix += 8) {

	// Divides xPix and yPix by 8 to the get the tile locations
	// and then multiplied yPix by 32 to ensure that we are looking at the
	// right row
	uint16_t tileLoc = tileMapAddr + ((xPix >> 3) | ((yPix >> 3) << 5));
	uint16_t tileNum = vram[tileLoc];
	uint16_t rowAddr;
	if (tileBlock == TILE_BLOCK_1) {
	    rowAddr = tileNum << 4;
	} else {
	    rowAddr = (tileNum & 0x80) ? (((tileNum & 0x7f) << 4) + 0x800) : ((tileNum << 4) + 0x1000);
	}

	rowAddr += (yPix & 0x7) << 1;
	uint8_t botByte = vram[rowAddr];
	uint8_t topByte = vram[rowAddr + 1];

	for(int i = 0; i < 8; i++) {
	    uint8_t curPix = xPix + i;
	    if (curPix >= xStart && curPix < endLine) {
		int color = (((topByte >> i) & 0x1) << 1) | ((botByte >> i) & 0x1);
		screen[line][curPix - xStart] = bgPalette[color];
	    }
	}
	
    }
}

void drawLine() {
    uint8_t lcdc = readLCD(0xff40);
    uint8_t line = readLCD(0xff44);

    drawBgLine(lcdc, line);

    return;
}

void drawFrame() {
    renderFrame(screen);
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

