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

struct rtcRegs{
    uint8_t sec;
    uint8_t min;
    uint8_t hr;
    // The day is the zero bit in the flags and then the 8 bits from the day reg
    uint8_t day;
    uint8_t flags;
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
    bool bankingMode;
    // MBC3 Specific items
    bool rtcAvail;
    struct rtcRegs curRtcRegs;
    struct rtcRegs latchedRegs;
    uint8_t prevLatch;

    uint8_t *ram;
    uint8_t *curRamBank;
    uint8_t *curRomBank;
    // This is for mode 1 (in MBC1 for now)
    uint8_t *curRomBank0;
    uint16_t numRomBanks;
    uint8_t numRamBanks;
    uint16_t curRomBankNum;
    uint8_t curRamBankNum;
} rom_t;

typedef uint8_t (*memMapRead_t)(uint16_t);
typedef void (*memMapWrite_t)(uint16_t, uint8_t);

uint8_t romRead(uint16_t addr);
void romWrite(uint16_t addr, uint8_t val);

void cartInit(char *file);

void updateRtcRegs();

#endif

