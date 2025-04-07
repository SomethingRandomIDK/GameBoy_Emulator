#include "./include/ppu.h"
#include "./include/gui.h"
#include "./include/lcd.h"
#include <stdint.h>

static uint8_t vram[0x2000];
static uint8_t oam[0xa0];
static uint8_t screen[144][160];

static uint8_t palette[4] = {0xff, 0xab, 0x55, 0x00};

static uint8_t windowLine = 0;

enum tileData_t{
    TILE_BLOCK_1,
    TILE_BLOCK_2
};

void resetWindowLine() {
    windowLine = 0;
}

static void drawWinLine(uint8_t lcdc, uint8_t line) {
    uint8_t bgPaletteNum = readLCD(0xff47);
    uint8_t bgPalette[4] = {
	palette[bgPaletteNum & 0x3],
	palette[(bgPaletteNum >> 2) & 0x3],
	palette[(bgPaletteNum >> 4) & 0x3],
	palette[(bgPaletteNum >> 6) & 0x3]
    };

    uint8_t yWin = readLCD(0xff4a);
    uint8_t xWin = readLCD(0xff4b);

    if (!(lcdc & 0x1 && lcdc & 0x20) || line < yWin || xWin > 166) {
	return;
    }

    uint16_t tileMapAddr = (lcdc & 0x40) ? 0x1c00 : 0x1800;
    enum tileData_t tileBlock = (lcdc & 0x10) ? TILE_BLOCK_1 : TILE_BLOCK_2;

    uint16_t xPix = xWin;

    for (; xPix < 167; xPix += 8) {
	uint16_t xTile = xPix - xWin; 

	uint16_t tileLoc = tileMapAddr + ((xTile >> 3) | ((windowLine >> 3) << 5));
	uint16_t tileNum = vram[tileLoc];
	uint16_t rowAddr;
	if (tileBlock == TILE_BLOCK_1) {
	    rowAddr = tileNum << 4;
	} else {
	    rowAddr = (tileNum & 0x80) ? (((tileNum & 0x7f) << 4) + 0x800) : ((tileNum << 4) + 0x1000);
	}

	rowAddr += (windowLine & 0x7) << 1;
	uint8_t botByte = vram[rowAddr];
	uint8_t topByte = vram[rowAddr + 1];

	for(int i = 0; i < 8; i++) {
	    uint16_t curPix = xPix + i;
	    if (curPix >= 7 && curPix < 167) {
		int color = (((topByte >> (7 - i)) & 0x1) << 1) | ((botByte >> (7 - i)) & 0x1);
		screen[line][curPix - 7] = bgPalette[color];
	    }
	}
    }
    windowLine++;
}

static void drawBgLine(uint8_t lcdc, uint8_t line) {
    uint8_t bgPaletteNum = readLCD(0xff47);
    uint8_t bgPalette[4] = {
	palette[bgPaletteNum & 0x3],
	palette[(bgPaletteNum >> 2) & 0x3],
	palette[(bgPaletteNum >> 4) & 0x3],
	palette[(bgPaletteNum >> 6) & 0x3]
    };

    if (!(lcdc & 0x01)) {
	for (int i = 0; i < 160; i++) {
	    screen[line][i] = bgPalette[0];
	}
	return;
    }

    // Actual map values are 0x9800 and 0x9c00, but we are going to be directly
    // accessing the vram
    uint16_t tileMapAddr = (lcdc & 0x8) ? 0x1c00 : 0x1800;
    enum tileData_t tileBlock = (lcdc & 0x10) ? TILE_BLOCK_1 : TILE_BLOCK_2;

    uint8_t yPix = readLCD(0xff42) + line;
    uint8_t xStart = readLCD(0xff43);
    uint16_t endLine = xStart + 160;
    // Sets xPix to the start of the tile
    uint16_t xPix = xStart & 0xfff8;

    for (;xPix < endLine; xPix += 8) {
	uint8_t xRow = (xPix & 0xff);

	// Divides xPix and yPix by 8 to the get the tile locations
	// and then multiplied yPix by 32 to ensure that we are looking at the
	// right row
	uint16_t tileLoc = tileMapAddr + ((xRow >> 3) | ((yPix >> 3) << 5));
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
	    uint16_t curPix = xPix + i;
	    if (curPix >= xStart && curPix < endLine) {
		int color = (((topByte >> (7 - i)) & 0x1) << 1) | ((botByte >> (7 - i)) & 0x1);
		screen[line][curPix - xStart] = bgPalette[color];
	    }
	}
	
    }
}

void drawLine() {
    uint8_t lcdc = readLCD(0xff40);
    uint8_t line = readLCD(0xff44);

    drawBgLine(lcdc, line);
    drawWinLine(lcdc, line);
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

