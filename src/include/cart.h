#ifndef _CART_H_
#define _CART_H_

#include <stdint.h>
#include <stdlib.h>

struct romHeader{
    // size of 4
    uint8_t *entry;
    // size of 0x30
    uint8_t *logo;
    // size of 0x10
    char *title;
    uint16_t newLicenseeCode;
    uint8_t sgbFlag;
    uint8_t type;
    uint8_t romSize;
    uint8_t ramSize;
    uint8_t dest;
    uint8_t oldLicenseeCode;
    uint8_t version;
    uint8_t checksum;
    uint16_t globalChecksum;
};

typedef struct {
    struct romHeader header;
    char *filename;
    size_t filenameSize;
    uint8_t *cartridge;
    size_t cartSize;
} rom_t;

void cartInit(char *file);

#endif

