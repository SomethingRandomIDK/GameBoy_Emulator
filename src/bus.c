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

#include "./include/cart.h"
#include "./include/bus.h"
#include "./include/cpu.h"
#include "./include/io.h"
#include "./include/ppu.h"

bool booting = true;

// Copy a version of the gameboy boot rom here
static uint8_t bootRom[0x100] = {
};

uint8_t busRead8(uint16_t addr) {
    if (addr < 0x8000) {
        // Read from cart
        return romRead(addr);
    } else if (addr < 0xa000) {
        // Read from vram
        return ppuVramRead(addr);
    } else if (addr < 0xc000) {
        // Read from cart (RAM)
        return romRead(addr);
    } else if (addr < 0xe000) {
        // read from WRAM
        return wramRead(addr);
    } else if (addr < 0xfe00) {
        // Echo Ram but it wraps around so reading or writing from here should
        // do the same thing as reading and writing to 0xc000 to 0xddff
        return wramRead(addr - 0x2000);
    } else if (addr < 0xfea0) {
        // OAM (Object attribute memory) I think this is where the sprites are
        // saved
	return ppuOamRead(addr);
    } else if (addr < 0xff00) {
        // Nintendo says that using this space is prohibited, but there is still
        // some stuff that happens when you do use it
        // When the OAM is blocked it just returns 0xff
        // Due to a bug it is possible to trigger OAM corruption (NEED to do 
        // more research)
        return 0;
    } else if (addr < 0xff80) {
        // IO Registers
        return ioRead(addr);
    } else if (addr < 0xffff) {
        // HRAM
        return hramRead(addr);
    } else {
        // Interupt register
        return ioRead(addr);
    }
    return 0;
}

void busWrite8(uint16_t addr, uint8_t val){
    if (addr < 0x8000) {
        // Write from cart
        romWrite(addr, val);
    } else if (addr < 0xa000) {
        // Write from vram
	ppuVramWrite(addr, val);
    } else if (addr < 0xc000) {
        // Write from cart (RAM)
        romWrite(addr, val);
    } else if (addr < 0xe000) {
        // Write from WRAM
        wramWrite(addr, val);
    } else if (addr < 0xfe00) {
        // Echo Ram but it wraps around so reading or writing from here should
        // do the same thing as reading and writing to 0xc000 to 0xddff
        wramWrite(addr - 0x2000, val);
    } else if (addr < 0xfea0) {
        // OAM (Object attribute memory) I think this is where the sprites are
        // saved
	ppuOamWrite(addr, val);
    } else if (addr < 0xff00) {
        // Nintendo says that using this space is prohibited, but there is still
        // some stuff that happens when you do use it
        // When the OAM is blocked it just returns 0xff
        // Due to a bug it is possible to trigger OAM corruption (NEED to do 
        // more research)
        return;
    } else if (addr < 0xff80) {
        // IO Registers
        ioWrite(addr, val);
    } else if (addr < 0xffff) {
        // HRAM
        hramWrite(addr, val);
    } else {
        // Interupt register
        ioWrite(addr, val);
    }
    return;
}

uint16_t busRead16(uint16_t addr) {
    uint16_t lo = busRead8(addr);
    uint16_t up = busRead8(addr + 1);
    return (up << 8) | lo;
}

void busWrite16(uint16_t addr, uint16_t val){
    busWrite8(addr, val & 0xff);
    busWrite8(addr + 1, (val >> 8) & 0xff);
}

