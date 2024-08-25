#include <stdio.h>
#include <stdlib.h>

#include "./include/cart.h"

struct romHeader header;

void cartInit(char *file) {
    uint8_t *cartridge;
    size_t cartSize;

    FILE *f = fopen(file, "r");
    if (f == NULL) {
	printf("File Failed to Open\n");
	exit(1);
    }

    fseek(f, 0, SEEK_END);
    cartSize = ftell(f);
    rewind(f);
    cartridge = malloc(cartSize);
    fread(cartridge, cartSize, 1, f);
    fclose(f);

    header.entry = (uint8_t *)(cartridge + 0x100);
    header.logo = (uint8_t *)(cartridge + 0x104);
    header.title = (char *)(cartridge + 0x134);

    header.title[0xf] = 0;

    header.newLicenseeCode = (*(cartridge + 0x144) << 8) | (*(cartridge + 0x145));
    header.sgbFlag = *(cartridge + 0x146);
    header.type = *(cartridge + 0x147);
    header.romSize = *(cartridge + 0x148);
    header.ramSize = *(cartridge + 0x149);
    header.dest = *(cartridge + 0x14a);
    header.oldLicenseeCode = *(cartridge + 0x14b);
    header.version = *(cartridge + 0x14c);
    header.checksum = *(cartridge + 0x14d);
    header.globalChecksum = (*(cartridge + 0x14e) << 8) | (*(cartridge + 0x14f));

    free(cartridge);
}

