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

#include "./include/io.h"
#include "./include/lcd.h"
#include "./include/interrupt.h"
#include "./include/timer.h"
#include "./include/serial.h"
#include "./include/gui.h"
#include "./include/apu.h"

uint8_t ioRead(uint16_t addr) {
    if (addr > 0xff3f && addr < 0xff4c) {
	return readLCD(addr);
    }

    if (addr > 0xff0f && addr < 0xff40) {
        return readSound(addr);
    }

    switch(addr) {
	case 0xff00:
	    return readJoypad();
        case 0xff01:
            return readSB();
        case 0xff02:
            return readSC();
        case 0xff04:
        case 0xff05:
        case 0xff06:
        case 0xff07:
            return readTimer(addr);
        case 0xff0f:
            return interFlags;
        case 0xffff:
            return interEnable;
        default:
            return 0xff;
    }
}

void ioWrite(uint16_t addr, uint8_t val) {
    if (addr > 0xff3f && addr < 0xff4c) {
	writeLCD(addr, val);
    }

    if (addr > 0xff0f && addr < 0xff40) {
        writeSound(addr, val);
        return;
    }

    switch(addr) {
	case 0xff00:
	    writeJoypad(val);
	    break;
        case 0xff01:
            writeSB(val);
            break;
        case 0xff02:
            writeSC(val);
            break;
        case 0xff04:
        case 0xff05:
        case 0xff06:
        case 0xff07:
            writeTimer(addr, val);
            break;
        case 0xff0f:
            interFlags = (val | 0xe0);
            break;
        case 0xffff:
            interEnable = val;
            break;
    }
}

