#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "./include/cpu.h"
#include "./include/cart.h"

int main(int argv, char **argc) {
    initCPU();
    // Testing for registers
    assert(0xaa00 == regAF());
    assert(0xbbcc == regBC());
    assert(0xddee == regDE());
    assert(0x1122 == regHL());

    setAF(0x1199);
    assert(0x1199 == regAF());
    setBC(0x1199);
    assert(0x1199 == regBC());
    setDE(0x1199);
    assert(0x1199 == regDE());
    setHL(0x1199);
    assert(0x1199 == regHL());

    // Testing Flags
    assert(flagZ());
    assert(!flagN());
    assert(!flagH());
    assert(flagC());

    if (argv == 2)
        cartInit(argc[1]);
    else {
        printf("This program requires one cli input\n");
        exit(1);
    }

    startCPU();

    return 0;
}

