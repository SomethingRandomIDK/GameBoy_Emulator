#include "./include/cart.h"
#include "./include/bus.h"
#include "./include/cpu.h"

uint8_t busRead8(uint16_t addr) {
    if (addr < 0x8000) {
        // Read from cart
        return romRead(addr);
    } else if (addr < 0xa000) {
        // Read from vram
        return 0;
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
        return 0;
    } else if (addr < 0xff00) {
        // Nintendo says that using this space is prohibited, but there is still
        // some stuff that happens when you do use it
        // When the OAM is blocked it just returns 0xff
        // Due to a bug it is possible to trigger OAM corruption (NEED to do 
        // more research)
        return 0;
    } else if (addr < 0xff80) {
        // IO Registers
        return 0;
    } else if (addr < 0xffff) {
        // HRAM
        return hramRead(addr);
    } else {
        // Interupt register
        return 0;
    }
    return 0;
}

void busWrite8(uint16_t addr, uint8_t val){
    if (addr < 0x8000) {
        // Write from cart
        romWrite(addr, val);
    } else if (addr < 0xa000) {
        // Write from vram
        return;
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
        return;
    } else if (addr < 0xff00) {
        // Nintendo says that using this space is prohibited, but there is still
        // some stuff that happens when you do use it
        // When the OAM is blocked it just returns 0xff
        // Due to a bug it is possible to trigger OAM corruption (NEED to do 
        // more research)
        return;
    } else if (addr < 0xff80) {
        // IO Registers
        return;
    } else if (addr < 0xffff) {
        // HRAM
        hramWrite(addr, val);
    } else {
        // Interupt register
        return;
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

