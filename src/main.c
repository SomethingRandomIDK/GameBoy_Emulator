#include <assert.h>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "./include/cpu.h"

int main(int argv, char **argc) {
    initCPU();
    assert(0xaaff == regAF());
    assert(0xbbcc == regBC());
    assert(0xddee == regDE());
    assert(0x1122 == regHL());

    printf("This compiles fine\n");
    return 0;
}

