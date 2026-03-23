/*
    gbemu - A Cross-Platform Game Boy Emulator
    Copyright (C) 2024-2026  Mehul Tomar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "./include/lcd.h"
#include "./include/interrupt.h"
#include "./include/bus.h"
#include "./include/gui.h"
#include "./include/ppu.h"

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
static uint8_t statInter = 0;

static uint8_t lcdRegs[] = {
    0x91, 0x81, 0x00, 0x00, 0x91, 0x00, 0xff, 0xfc, 0xff, 0xff, 0x00, 0x00
};

bool dmaTransfering() {
    return dmaTransfer;
}

static void checkLy() {
    if (lcdRegs[LY] == lcdRegs[LYC]) {
        lcdRegs[STAT] |= 0x4;
        if (lcdRegs[STAT] & 0x40) {
            if (statInter == 0) {
                raiseInterrupt(LCD);
            }
            statInter |= 0x8;
        }
    } else {
        lcdRegs[STAT] &= ~(0x4);
        statInter &= ~(0x8);
    }
}

void incLCDTimer(uint32_t cycles) {
    dmaClock += cycles;
    while (dmaClock > 3 && dmaTransfer) {
        dmaWrite((0xfe00 | lowerAddr), busRead8((upperAddr << 8) | lowerAddr));
        lowerAddr++;
        dmaClock -= 4;
        dmaTransfer = lowerAddr < 0xa0;
    }

    if (lcdRegs[LCDC] & 0x80) {
        lcdClock += cycles;
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
                            if (!(statInter & 0x9)) {
                                raiseInterrupt(LCD);
                            }
                            statInter |= 0x2;
                        }
                        statInter &= 0xe;

                        // TODO Draw Screen Here and wait
                        drawFrame();

                    } else {
                        statInter &= 0x9;
                        lcdRegs[STAT] = (lcdRegs[STAT] & ~(0x3)) | 0x2;

                        if (lcdRegs[STAT] & 0x20) {
                            if (statInter == 0) {
                                raiseInterrupt(LCD);
                            }
                            statInter |= 0x4;
                        }
                        statInter &= 0xe;
                    }
                }
                break;
            case 1: // Vertical Blank
                if (lcdClock >= 456) {
                    lcdClock -= 456;
                    lcdRegs[LY]++;
                    if (lcdRegs[LY] >= 154) {
                        lcdRegs[LY] = 0;
                        resetWindowLine();
                        statInter &= 0x7;
                        checkLy();
                        statInter &= 0xa;
                        lcdRegs[STAT] = (lcdRegs[STAT] & (~(0x3))) | 0x2;

                        if (lcdRegs[STAT] & 0x20) {
                            if (statInter == 0) {
                                raiseInterrupt(LCD);
                            }
                            statInter |= 0x4;
                        }
                        statInter &= 0xd;
                    } else {
                        checkLy();
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

                    // TODO Possibly draw the line here at the end of the Mode 3
                    drawLine();

                    lcdRegs[STAT] = lcdRegs[STAT] & ~(0x3) | 0x0;
                    if (lcdRegs[STAT] & 0x08) {
                        raiseInterrupt(LCD);
                    }
                }
                break;
        }
    }
}

uint8_t readLCD(uint16_t addr) {
    return lcdRegs[addr - 0xff40];
}

void writeLCD(uint16_t addr, uint8_t val) {
    switch(addr - 0xff40) {
        case LCDC:
            if (!(val & 0x80)) {
                lcdRegs[STAT] &= 0x7c;
                lcdRegs[LY] = 0;
                resetWindowLine();
                lcdClock = 0;
                statInter = 0;
            } else if ((val & 0x80) && !(lcdRegs[LCDC] & 0x80)) {
                lcdRegs[STAT] = (lcdRegs[STAT] & ~(0x3)) | 0x2;
                lcdClock = 0;
            }
            lcdRegs[LCDC] = val;
            break;
        case DMA:
            dmaTransfer = true;
            upperAddr = val;
            lowerAddr = 0;
            dmaClock = 0;
            break;
        case STAT:
            if (lcdRegs[LCDC] & 0x80) {
                if (((val & 0x08) > (lcdRegs[STAT] & 0x08)) && (lcdRegs[STAT] & 0x3) == 0) {
                    raiseInterrupt(LCD);
                }
                if (((val & 0x10) > (lcdRegs[STAT] & 0x10)) && (lcdRegs[STAT] & 0x3) == 1) {
                    raiseInterrupt(LCD);
                }
                if (((val & 0x20) > (lcdRegs[STAT] & 0x20)) && (lcdRegs[STAT] & 0x3) == 2) {
                    raiseInterrupt(LCD);
                }
                if ((val & 0x40) > (lcdRegs[STAT] & 0x40)) {
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

