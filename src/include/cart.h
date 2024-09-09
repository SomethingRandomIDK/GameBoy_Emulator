#ifndef _CART_H_
#define _CART_H_

#include <stdbool.h>
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

enum cartType {
    ROM,
    MBC1,
    MBC2,
    MBC3,
    MBC5,
    MBC6,
    MBC7,
    MMM01,
    HUC1,
    HUC3
};

typedef struct {
    struct romHeader header;
    char *filename;
    size_t filenameSize;
    uint8_t *cartridge;
    size_t cartSize;

    enum cartType cType;
    bool ramAvail;
    // This will also act to determine whether the ram is ram or IR register, 
    // with ram indicated by true and IR register indicated by false
    bool ramEnable;
    uint8_t *ram;
    uint8_t *curRamBank;
    uint8_t *curRomBank;
    int numRomBanks;
    int curRamNum;
} rom_t;

typedef uint8_t (*memMapRead_t)(uint16_t);
typedef void (*memMapWrite_t)(uint16_t, uint8_t);

uint8_t romRead(uint16_t addr);
void romWrite(uint16_t addr, uint8_t val);

void cartInit(char *file);

#endif

