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

#include <assert.h>
#include <stdio.h>

#include "./include/cpu.h"
#include "./include/cart.h"

int main(int argc, char **argv) {
    #ifdef NDEBUG
    printf("GBEmu: Release Build: v0.1.1\n");
    #else
    printf("GBEmu: Debug Build: v0.1.1\n");
    #endif
    if (argc == 2)
        cartInit(argv[1]);
    else {
        printf("USAGE: %s <path to rom file>\n", argv[0]);
        exit(1);
    }

    initCPU();

    // Testing for registers
    assert(0x01b0 == regAF());
    assert(0x0013 == regBC());
    assert(0x00d8 == regDE());
    assert(0x014d == regHL());

    startCPU();

    return 0;
}

