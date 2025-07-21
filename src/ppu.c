#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>

#include "./include/ppu.h"
#include "./include/gui.h"
#include "./include/lcd.h"

static uint8_t vram[0x2000];
static uint8_t oam[0xa0];
static uint8_t screen[144][160];
static uint8_t bgScreen[144][160];

static uint8_t palette[4] = {0xff, 0xab, 0x55, 0x00};

static uint8_t windowLine = 0;

enum tileData_t{
    TILE_BLOCK_1,
    TILE_BLOCK_2
};

// Sort all sprites by x position using stable sort to not mess up the priority from reading
static void sortSprites(int spritePriority[10][2], int oamNum) {
    int i, j, k, m;
    for (i = 1; i < oamNum; i++) {
	m = spritePriority[i][0];
	k = spritePriority[i][1];
	j = i - 1;

	while (j >= 0 && k < spritePriority[j][1]) {
	    spritePriority[j + 1][0] = spritePriority[j][0];
	    spritePriority[j + 1][1] = spritePriority[j][1];
	    j--;
	}
	spritePriority[j + 1][0] = m;
	spritePriority[j + 1][1] = k;
    }
}
 
static void drawSpriteLine(uint8_t lcdc, uint8_t line) {
    if (!(lcdc & 0x2)) {
	 return;
    }
    uint8_t obj0PaletteNum = readLCD(0xff48);
    uint8_t obj1PaletteNum = readLCD(0xff49);

    uint8_t obj0[4] = {
	palette[obj0PaletteNum & 0x3],
	palette[(obj0PaletteNum >> 2) & 0x3],
	palette[(obj0PaletteNum >> 4) & 0x3],
	palette[(obj0PaletteNum >> 6) & 0x3]
    };
    uint8_t obj1[4] = {
	palette[obj1PaletteNum & 0x3],
	palette[(obj1PaletteNum >> 2) & 0x3],
	palette[(obj1PaletteNum >> 4) & 0x3],
	palette[(obj1PaletteNum >> 6) & 0x3]
    };

    bool size = !!(lcdc & 0x4);
    int oamIdx = 0;
    int oamNum = 0;
    // idx 0 would be the oam index and idx 1 would be the x value
    int oamPriority[10][2];

    for(int i = 0; i < 0xa0; i += 4) {
	if (size && line >= (oam[i] - 16) && line < oam[i]) {
	    oamPriority[oamNum][0] = oamIdx;
	    oamPriority[oamNum][1] = oam[i + 1];
	    oamNum++;
	    if (oamNum == 10) break;
	} else if (!size && line >= (oam[i] - 16) && line < (oam[i] - 8)){
	    oamPriority[oamNum][0] = oamIdx;
	    oamPriority[oamNum][1] = oam[i + 1];
	    oamNum++;
	    if (oamNum == 10) break;
	}
	oamIdx++;
    }

    sortSprites(oamPriority, oamNum);

    uint8_t spriteLine[160] = {0};

    for (int i = oamNum - 1; i >= 0; i--) {
	uint8_t spriteIdx = oamPriority[i][0] << 2;
	uint8_t yPos = oam[spriteIdx];
	uint8_t xPos = oam[spriteIdx + 1];
	uint8_t tileNum = oam[spriteIdx + 2];
	uint8_t attr = oam[spriteIdx + 3];

	if (size) {
	    tileNum &= ~(0x1);
	}

	uint16_t tileLoc = tileNum << 4;
	uint16_t rowAddr;
	if (attr & 0x40) {
	    if (size) {
		rowAddr = yPos - line - 1;
	    } else {
		rowAddr = yPos - line - 9;
	    }
	} else {
	    rowAddr = line + 16 - yPos;
	}

	rowAddr = rowAddr << 1;
	rowAddr += tileLoc;

	uint8_t botByte = vram[rowAddr];
	uint8_t topByte = vram[rowAddr + 1];

	for (int j = 0; j < 8; j++) {
	    int curPix = xPos + j - 8;
	    if (curPix >= 0 && curPix < 160) {
		uint8_t color;
		if (attr & 0x20) {
		    color = (((topByte >> j) & 0x1) << 1) | ((botByte >> j) & 0x1);
		} else {
		    color = (((topByte >> (7 - j)) & 0x1) << 1) | ((botByte >> (7 - j)) & 0x1);
		}

		uint8_t pix = 0;
		pix |= (attr & 0x80);
		pix |= (attr & 0x10);
		pix |= (color & 0x03);

		if (color)
		spriteLine[curPix] = pix;
	    }
	}
    }

    for (int i = 0; i < 160; i++) {
	if (spriteLine[i] & 0x80) {
	    if (!bgScreen[line][i] && !!(spriteLine[i] & 0x3)) {
		screen[line][i] = (spriteLine[i] & 0x10) ? (obj1[(spriteLine[i] & 0x3)]): (obj0[(spriteLine[i] & 0x3)]);
	    }
	} else if (spriteLine[i] & 0x3) {
		screen[line][i] = (spriteLine[i] & 0x10) ? (obj1[(spriteLine[i] & 0x3)]): (obj0[(spriteLine[i] & 0x3)]);
	}
    }
}

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
		bgScreen[line][curPix - 7] = color;
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
		bgScreen[line][curPix - xStart] = color;
	    }
	}
	
    }
}

void drawLine() {
    uint8_t lcdc = readLCD(0xff40);
    uint8_t line = readLCD(0xff44);

    drawBgLine(lcdc, line);
    drawWinLine(lcdc, line);
    drawSpriteLine(lcdc, line);
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

