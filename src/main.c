#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "./include/cpu.h"
#include "./include/cart.h"

int main(int argv, char **argc) {
    initCPU();
    // Testing for registers
    assert(0xaaff == regAF());
    assert(0xbbcc == regBC());
    assert(0xddee == regDE());
    assert(0x1122 == regHL());

    // Testing Flags
    assert(flagZ());
    assert(flagN());
    assert(flagH());
    assert(flagC());

    if (argv == 2)
	cartInit(argc[1]);
    else {
	printf("This program requires one cli input\n");
	exit(1);
    }

    return 0;
}

