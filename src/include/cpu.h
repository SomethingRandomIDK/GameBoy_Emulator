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

#ifndef _CPU_H_
#define _CPU_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t a;
    uint8_t b;
    uint8_t c;
    uint8_t d;
    uint8_t e;
    uint8_t h;
    uint8_t l;
    uint8_t f;
    uint16_t sp;
    uint16_t pc;
} registers_t;

typedef struct {
    registers_t regs;
    uint8_t wram[0x2000];
    uint8_t hram[0x7f];
    bool interrupts;
    bool interEnableBuffer;
    bool halted;
    bool stopped;
} gb_t;

uint16_t regAF();
uint16_t regBC();
uint16_t regDE();
uint16_t regHL();

void setAF(uint16_t val);
void setBC(uint16_t val);
void setDE(uint16_t val);
void setHL(uint16_t val);

bool flagZ();
bool flagN();
bool flagH();
bool flagC();

void setZ(bool newZ);
void setN(bool newN);
void setH(bool newH);
void setC(bool newC);

uint8_t wramRead(uint16_t addr);
void wramWrite(uint16_t addr, uint8_t val);

uint8_t hramRead(uint16_t addr);
void hramWrite(uint16_t addr, uint8_t val);

void initCPU(void);

void startCPU(void);

void stopRunning();

#endif

