#include <assert.h>
#include <stdio.h>

#include "./include/cpu.h"
#include "./include/cart.h"

int main(int argc, char **argv) {
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

