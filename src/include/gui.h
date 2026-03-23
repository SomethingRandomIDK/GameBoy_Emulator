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

#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

#define SAMPLE_SIZE 1024
#define GUI_FREQUENCY 44100

// This was found by gameboy CPU clock speed (4194304) / GUI_FREQUENCY
#define CLOCKS_PER_SAMPLE 95

// This is 160*144 for the total number of pixels on screen
#define SCREEN_SIZE 23040

uint8_t readJoypad();
void writeJoypad(uint8_t val);

bool checkStop();

void initGUI();
void closeGUI();

void incEventTimer(uint32_t cycles);

void renderFrame(uint32_t screen[SCREEN_SIZE]);

void pushAudio(uint8_t sPanning, uint8_t mVol, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4);

#endif

