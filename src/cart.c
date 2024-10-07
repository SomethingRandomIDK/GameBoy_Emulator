#include <stdio.h>
#include <string.h>

#include "./include/cart.h"

rom_t rom;

static const char *newLicenseeText() {
    switch(rom.header.newLicenseeCode) {
	case 0x3030:
	    return "None";
	case 0x3031:
	    return "Nintendo Research & Development 1";
	case 0x3038:
	    return "Capcom";
	case 0x3133:
	    return "EA (Electronic Arts)";
	case 0x3138:
	    return "Hudson Soft";
	case 0x3139:
	    return "B-AI";
	case 0x3230:
	    return "KSS";
	case 0x3232:
	    return "Planning Office WADA";
	case 0x3234:
	    return "PCM Complete";
	case 0x3235:
	    return "San-X";
	case 0x3238:
	    return "Kemco";
	case 0x3239:
	    return "SETA Corporation";
	case 0x3330:
	    return "Viacom";
	case 0x3331:
	    return "Nintendo";
	case 0x3332:
	    return "Bandai";
	case 0x3333:
	    return "Ocean Software//Acclaim Entertainment";
	case 0x3334:
	    return "Konami";
	case 0x3335:
	    return "HectorSoft";
	case 0x3337:
	    return "Taito";
	case 0x3338:
	    return "Hudson Soft";
	case 0x3339:
	    return "Banpresto";
	case 0x3431:
	    return "Ubi Soft";
	case 0x3432:
	    return "Atlus";
	case 0x3434:
	    return "Malibu Interactive";
	case 0x3436:
	    return "Angel";
	case 0x3437:
	    return "Bullet-Proof Software";
	case 0x3439:
	    return "Irem";
	case 0x3530:
	    return "Absolute";
	case 0x3531:
	    return "Acclaim Entertainment";
	case 0x3532:
	    return "Activision";
	case 0x3533:
	    return "Sammy USA Corporation";
	case 0x3534:
	    return "Konami";
	case 0x3535:
	    return "Hi Tech Expression";
	case 0x3536:
	    return "LJN";
	case 0x3537:
	    return "Matchbox";
	case 0x3538:
	    return "Mattel";
	case 0x3539:
	    return "Milton Bradley Company";
	case 0x3630:
	    return "Titus Interactive";
	case 0x3631:
	    return "Virgin Games Ltd.";
	case 0x3634:
	    return "Lucasfilm Games";
	case 0x3637:
	    return "Ocean Software";
	case 0x3639:
	    return "EA (Electronic Arts)";
	case 0x3730:
	    return "Infogrames";
	case 0x3731:
	    return "Interplay Entertainment";
	case 0x3732:
	    return "Broderbund";
	case 0x3733:
	    return "Sculptured Software";
	case 0x3735:
	    return "The Sales Curve Limited";
	case 0x3738:
	    return "THQ";
	case 0x3739:
	    return "Accolade";
	case 0x3830:
	    return "Misawa Entertainment";
	case 0x3833:
	    return "lozc";
	case 0x3836:
	    return "Tokuma Shoten";
	case 0x3837:
	    return "Tsukuda Original";
	case 0x3931:
	    return "Chunsoft Co.";
	case 0x3932:
	    return "Video System";
	case 0x3933:
	    return "Ocean Software/Acclaim Entertainment";
	case 0x3935:
	    return "Varie";
	case 0x3936:
	    return "Yonezawa/S'pal";
	case 0x3937:
	    return "Kaneko";
	case 0x3939:
	    return "Pack-In-Video";
	case 0x3948:
	    return "Bottom Up";
	case 0x4134:
	    return "Konami (Yu-Gi-Oh!)";
	case 0x424c:
	    return "MTO";
	case 0x444b:
	    return "Kodansha";
	default:
	    return "Not Found";
    }
}

static const char *licenseeText() {
    switch(rom.header.oldLicenseeCode) {
	case 0x00:
	    return "None";
	case 0x01:
	    return "Nintendo";
	case 0x08:
	    return "Capcom";
	case 0x09:
	    return "HOT-B";
	case 0x0a:
	    return "Jaleco";
	case 0x0b:
	    return "Coconuts Japan";
	case 0x0c:
	    return "Elite Systems";
	case 0x13:
	    return "EA (Electronic Arts)";
	case 0x18:
	    return "Hudson Soft";
	case 0x19:
	    return "ITC Entertainment";
	case 0x1a:
	    return "Yanoman";
	case 0x1d:
	    return "Japan Clary";
	case 0x1f:
	    return "Virgin Games Ltd.";
	case 0x24:
	    return "PCM Complete";
	case 0x25:
	    return "San-X";
	case 0x28:
	    return "Kemco";
	case 0x29:
	    return "SETA Corporation";
	case 0x30:
	    return "Infogrames";
	case 0x31:
	    return "Nintendo";
	case 0x32:
	    return "Bandai";
	case 0x33:
	    return newLicenseeText();
	case 0x34:
	    return "Konami";
	case 0x35:
	    return "HectorSoft";
	case 0x38:
	    return "Capcom";
	case 0x39:
	    return "Banpresto";
	case 0x3c:
	    return ".Entertainment i";
	case 0x3e:
	    return "Gremlin";
	case 0x41:
	    return "Ubi Soft";
	case 0x42:
	    return "Atlus";
	case 0x44:
	    return "Malibu Interactive";
	case 0x46:
	    return "Angel";
	case 0x47:
	    return "Spectrum Hobby";
	case 0x49:
	    return "Irem";
	case 0x4a:
	    return "Virgin Games Ltd.";
	case 0x4d:
	    return "Malibu Interactive";
	case 0x4f:
	    return "U.S. Gold";
	case 0x50:
	    return "Absolute";
	case 0x51:
	    return "Acclaim Entertainment";
	case 0x52:
	    return "Activision";
	case 0x53:
	    return "Sammy USA Corporation";
	case 0x54:
	    return "GameTek";
	case 0x55:
	    return "Park Place";
	case 0x56:
	    return "LJN";
	case 0x57:
	    return "Matchbox";
	case 0x59:
	    return "Milton Bradley Company";
	case 0x5a:
	    return "Mindscape";
	case 0x5b:
	    return "Romstar";
	case 0x5c:
	    return "Naxat Soft";
	case 0x5d:
	    return "Tradewest";
	case 0x60:
	    return "Titus Interactive";
	case 0x61:
	    return "Virgin Games Ltd.";
	case 0x67:
	    return "Ocean Software";
	case 0x69:
	    return "EA (Electronic Arts)";
	case 0x6e:
	    return "Elite Systems";
	case 0x6f:
	    return "Electro Brain";
	case 0x70:
	    return "Infogrames";
	case 0x71:
	    return "Interplay Entertainment";
	case 0x72:
	    return "Broderbund";
	case 0x73:
	    return "Sculptured Software";
	case 0x75:
	    return "The Sales Curve Limited";
	case 0x78:
	    return "THQ";
	case 0x79:
	    return "Accolade";
	case 0x7a:
	    return "Triffix Entertainment";
	case 0x7c:
	    return "Microprose";
	case 0x7f:
	    return "Kemco";
	case 0x80:
	    return "Misawa Entertainment";
	case 0x83:
	    return "Lozc";
	case 0x86:
	    return "Tokuma Shoten";
	case 0x8b:
	    return "Bullet-Proof Software";
	case 0x8c:
	    return "Vic Tokai";
	case 0x8e:
	    return "Ape";
	case 0x8f:
	    return "I'Max";
	case 0x91:
	    return "Chunsoft Co.";
	case 0x92:
	    return "Video System";
	case 0x93:
	    return "Tsubaraya Productions";
	case 0x95:
	    return "Varie";
	case 0x96:
	    return "Yonezawa/S'Pal";
	case 0x97:
	    return "Kemco";
	case 0x99:
	    return "Arc";
	case 0x9a:
	    return "Nihon Bussan";
	case 0x9b:
	    return "Tecmo";
	case 0x9c:
	    return "Imagineer";
	case 0x9d:
	    return "Banpresto";
	case 0x9f:
	    return "Nova";
	case 0xa1:
	    return "Hori Electric";
	case 0xa2:
	    return "Bandai";
	case 0xa4:
	    return "Konami";
	case 0xa6:
	    return "Kawada";
	case 0xa7:
	    return "Takara";
	case 0xa9:
	    return "Technos Japan";
	case 0xaa:
	    return "Broderbund";
	case 0xac:
	    return "Toei Animation";
	case 0xad:
	    return "Toho";
	case 0xaf:
	    return "Namco";
	case 0xb0:
	    return "Acclaim Entertainment";
	case 0xb1:
	    return "ASCII Corporation or Nexsoft";
	case 0xb2:
	    return "Bandai";
	case 0xb4:
	    return "Square Enix";
	case 0xb6:
	    return "HAL Laboratory";
	case 0xb7:
	    return "SNK";
	case 0xb9:
	    return "Pony Canyon";
	case 0xba:
	    return "Culture Brain";
	case 0xbb:
	    return "Sunsoft";
	case 0xbd:
	    return "Sony Imagesoft";
	case 0xbf:
	    return "Sammy Corporation";
	case 0xc0:
	    return "Taito";
	case 0xc2:
	    return "Kemco";
	case 0xc3:
	    return "Square";
	case 0xc4:
	    return "Tokuma Shoten";
	case 0xc5:
	    return "Data East";
	case 0xc6:
	    return "Tonkinhouse";
	case 0xc8:
	    return "Koei";
	case 0xc9:
	    return "UFL";
	case 0xca:
	    return "Ultra";
	case 0xcb:
	    return "Vap";
	case 0xcc:
	    return "Use Corporation";
	case 0xcd:
	    return "Meldac";
	case 0xce:
	    return "Pony Canyon";
	case 0xcf:
	    return "Angel";
	case 0xd0:
	    return "Taito";
	case 0xd1:
	    return "Sofel";
	case 0xd2:
	    return "Quest";
	case 0xd3:
	    return "Sigma Enterprises";
	case 0xd4:
	    return "ASK Kodansha Co.";
	case 0xd6:
	    return "Naxat Soft";
	case 0xd7:
	    return "Copya System";
	case 0xd9:
	    return "Banpresto";
	case 0xda:
	    return "Tomy";
	case 0xdb:
	    return "LJN";
	case 0xdd:
	    return "NCS";
	case 0xde:
	    return "Human";
	case 0xdf:
	    return "Altron";
	case 0xe0:
	    return "Jaleco";
	case 0xe1:
	    return "Towa Chiki";
	case 0xe2:
	    return "Yutaka";
	case 0xe3:
	    return "Varie";
	case 0xe5:
	    return "Epcoh";
	case 0xe7:
	    return "Athena";
	case 0xe8:
	    return "Asmik Ace Entertainment";
	case 0xe9:
	    return "Natsume";
	case 0xea:
	    return "King Records";
	case 0xeb:
	    return "Atlus";
	case 0xec:
	    return "Epic/Sony Records";
	case 0xee:
	    return "IGS";
	case 0xf0:
	    return "A Wave";
	case 0xf3:
	    return "Extreme Entertainment";
	case 0xff:
	    return "LJN";
	default:
	    return "Not Found";
    }
}

static const char *typeText() {
    switch(rom.header.type){
	case 0x00:
	    return "ROM only";
	case 0x01:
	    return "MBC1";
	case 0x02:
	    return "MBC1+RAM";
	case 0x03:
	    return "MBC1+RAM+BATTERY";
	case 0x05:
	    return "MBC2";
	case 0x06:
	    return "MBC2+BATTERY";
	case 0x08:
	    return "ROM+RAM";
	case 0x09:
	    return "ROM+RAM+BATTERY";
	case 0x0b:
	    return "MMM01";
	case 0x0c:
	    return "MMM01+RAM";
	case 0x0d:
	    return "MMM01+RAM+BATTERY";
	case 0x0f:
	    return "MBC3+TIMER+BATTERY";
	case 0x10:
	    return "MBC3+TIMER+RAM+BATTERY";
	case 0x11:
	    return "MBC3";
	case 0x12:
	    return "MBC3+RAM";
	case 0x13:
	    return "MBC3+RAM";
	case 0x19:
	    return "MBC5";
	case 0x1a:
	    return "MBC5+RAM";
	case 0x1b:
	    return "MBC5+RAM+BATTERY";
	case 0x1c:
	    return "MBC5+RUMBLE";
	case 0x1d:
	    return "MBC5+RUMBLE+RAM";
	case 0x1e:
	    return "MBC5+RUMBLE+RAM+BATTERY";
	case 0x20:
	    return "MBC6";
	case 0x22:
	    return "MBC7+SENSOR+RUMBLE+RAM+BATTERY";
	case 0xfc:
	    return "POCKET CAMERA";
	case 0xfd:
	    return "BANDAI TAMA5";
	case 0xfe:
	    return "HuC3";
	case 0xff:
	    return "HuC1+RAM+BATTERY";
	default:
	    return "NOT FOUND";
    }
}

static const char *romSizeText() {
    switch(rom.header.romSize) {
	case 0x00:
	    return "32kb 2 banks (no banking)";
	case 0x01:
	    return "64kb 4 banks";
	case 0x02:
	    return "128kb 8 banks";
	case 0x03:
	    return "256kb 16 banks";
	case 0x04:
	    return "512kb 32 banks";
	case 0x05:
	    return "1mb 64 banks";
	case 0x06:
	    return "2mb 128 banks";
	case 0x07:
	    return "4mb 256 banks";
	case 0x08:
	    return "8mb 512 banks";
	// The following are from unofficial docs and should not actually be
	// used by roms
	case 0x52:
	    return "1.1mb 72 banks";
	case 0x53:
	    return "1.2mb 80 banks";
	case 0x54:
	    return "1.5mb 96 banks";
	default:
	    return "Not Found";
    }
}

static const char *ramSizeText() {
    switch(rom.header.ramSize) {
	// If cartride type does not equal ram it should default to 0x00
	case 0x00:
	    return "No RAM";
	// Unofficial docs have a 2kb ram chip however this was never official
	// used by Nintendo or in a cartridge
	case 0x01:
	    return "Unused";
	case 0x02:
	    return "8kb 1 bank";
	case 0x03:
	    return "32kb 4 bank of 8kb each";
	case 0x04:
	    return "128kb 16 bank of 8kb each";
	case 0x05:
	    return "64kb 8 bank of 8kb each";
	default:
	 return "Not Found";
    }
}

static const char *destText() {
    switch(rom.header.dest) {
	case 0x00:
	    return "Japan (and possibly overseas)";
	case 0x01:
	    return "Overseas only";
	default:
	    return "Not found";
    }
}

static void displayInfo() {
    printf("Title: %s\n", rom.header.title);
    printf("Publisher: %s\n", licenseeText());
    printf("SGB Flag: %d\n", rom.header.sgbFlag);
    printf("ROM type: %s\n", typeText());
    printf("ROM size: %s\n", romSizeText());
    printf("RAM size: %s\n", ramSizeText());
    printf("Destination: %s\n", destText());
    printf("Version: %d\n", rom.header.version);
    printf("Checksum: %d\n", rom.header.checksum);
    printf("Global Checksum: %d\n", rom.header.globalChecksum);
}

static void cartTypeSelector() {
    switch(rom.header.type) {
        case 0x00:
        case 0x08:
        case 0x09:
            rom.cType = ROM;
            break;
        case 0x01:
        case 0x02:
        case 0x03:
            rom.cType = MBC1;
            // Set the pointer for curRomBank to the first Rom Bank
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            // MBC1 specific
            rom.bankingMode = 0;
            break;
        case 0x05:
        case 0x06:
            rom.cType = MBC2;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0x0b:
        case 0x0c:
        case 0x0d:
            rom.cType = MMM01;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0x0f:
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
            rom.cType = MBC3;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0x19:
        case 0x1a:
        case 0x1b:
        case 0x1c:
        case 0x1d:
        case 0x1e:
            rom.cType = MBC5;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0x20:
            rom.cType = MBC6;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0x22:
            rom.cType = MBC7;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0xfe:
            rom.cType = HUC3;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        case 0xff:
            rom.cType = HUC1;
            rom.curRomBank0 = rom.cartridge;
            rom.curRomBank = rom.cartridge + 0x4000;
            rom.numRomBanks = 1 << rom.header.romSize;
            rom.curRomBankNum = 1;
            break;
        default:
            printf("INVALID ROM TYPE\n");
            exit(1);
    }

    switch(rom.header.type) {
        case 0x02:
        case 0x03:
        case 0x08:
        case 0x09:
        case 0x0c:
        case 0x0d:
        case 0x10:
        case 0x12:
        case 0x13:
        case 0x1a:
        case 0x1b:
        case 0x1d:
        case 0x1e:
        case 0x22:
        case 0xff:
            rom.ramAvail = true;
            rom.ramEnable = false;
            switch(rom.header.ramSize) {
                case 0x00:
                    rom.ramAvail = false;
                    break;
                case 0x02:
                    rom.ram = (uint8_t *)calloc(0x2000, 1);
                    rom.curRamBank = rom.ram;
                    rom.curRamBankNum = 0;
                    rom.numRamBanks = 1;
                    break;
                case 0x03:
                    rom.ram = (uint8_t *)calloc(0x4 * 0x2000, 1);
                    rom.curRamBank = rom.ram;
                    rom.curRamBankNum = 0;
                    rom.numRamBanks = 4;
                    break;
                case 0x04:
                    rom.ram = (uint8_t *)calloc(0x10 * 0x2000, 1);
                    rom.curRamBank = rom.ram;
                    rom.curRamBankNum = 0;
                    rom.numRamBanks = 16;
                    break;
                case 0x05:
                    rom.ram = (uint8_t *)calloc(0x8 * 0x2000, 1);
                    rom.curRamBank = rom.ram;
                    rom.curRamBankNum = 0;
                    rom.numRamBanks = 8;
                    break;
                default:
                    printf("INVALID RAM SIZE\n");
                    exit(1);
            }
            break;
        default:
            rom.ramAvail = false;
            break;
    }
}

// NO Mapper/ Just ROM read and write functions
static uint8_t mapperRomRead(uint16_t addr) {
    if (addr < 0x8000) {
        return rom.cartridge[addr];
    } else if (addr > 0x9fff && addr < 0xc000 && rom.ramAvail) {
        return rom.ram[addr - 0xa000];
    }
    return 0xff;
}

static void mapperRomWrite(uint16_t addr, uint8_t val) {
    if (addr > 0x9fff && addr < 0xc000 && rom.ramAvail)
        rom.ram[addr - 0xa000] = val;
}

// Read and Write function for the MBC1 cartridge
static uint8_t mapperMBC1Read(uint16_t addr) {
    if (addr < 0x4000){
        if (rom.bankingMode)
            return rom.curRomBank0[addr];
        else
            return rom.cartridge[addr];
    }
    else if (addr < 0x8000)
        return rom.curRomBank[addr - 0x4000];
    else if (addr > 0x9fff && addr < 0xc000) {
        if (rom.ramAvail && rom.ramEnable) {
            if (rom.bankingMode)
                return rom.curRamBank[addr - 0xa000];
            else
                return rom.ram[addr - 0xa000];
        }
        else
            return 0xff;
    }
    return 0xff;
}

static void mapperMBC1Write(uint16_t addr, uint8_t val) {
    if (addr < 0x2000) {
        rom.ramEnable = (val & 0xf) == 0xa;
    } else if (addr < 0x4000) {
        // There are a few unofficial docs that use values where this won't be
        // supported, but it's unofficial so I don't want to make support for it
        // rn

        // TODO: add implementation for banking modes
        int shiftAmt = 8 - rom.header.romSize + 1;
        rom.curRomBankNum = (val & (0xff >> (shiftAmt)));
        if (shiftAmt < 4) {
            rom.curRomBankNum &= 0x1f;
            rom.curRomBankNum |= ((rom.curRamBankNum & 0x3) << 5);
            rom.curRomBank0 = rom.cartridge + ((rom.curRomBankNum & 0x60) * 0x4000);
        }
        if ((rom.curRomBankNum & 0x1f) == 0) {
            rom.curRomBankNum += 1;
        }
        rom.curRomBank = rom.cartridge + (rom.curRomBankNum * 0x4000);
    } else if (addr < 0x6000) {
        rom.curRamBankNum = (val & 0x3);
        if (rom.ramAvail){
            rom.curRamBank = rom.ram + (rom.curRamBankNum * 0x2000);
        }
    } else if (addr < 0x8000) {
        rom.bankingMode = val & 0x1;
    } else if (addr > 0x9fff && addr < 0xc000) {
        if (rom.ramAvail && rom.ramEnable) {
            if (rom.bankingMode)
                rom.curRamBank[addr - 0xa000] = val;
            else
                rom.ram[addr - 0xa000] = val;
        }
    }
}

static uint8_t mapperMBC2Read(uint16_t addr) {
    if (addr < 0x4000) {
        return rom.cartridge[addr];
    } else if (addr < 0x8000) {
        return rom.curRomBank[addr - 0x4000];
    } else if (addr > 0x9fff && addr < 0xc000 && rom.ramAvail && rom.ramEnable) {
        addr &= 0x1ff;
        return rom.ram[addr] & 0xf;
    }
    return 0xff;
}

static void mapperMBC2Write(uint16_t addr, uint8_t val) {
    if (addr < 0x4000) {
        if (addr & 0x100) {
            if ((val & 0xf) == 0) {
                val++;
            }
            rom.curRomBank = rom.cartridge + ((val & 0xf) * 0x4000);
        } else {
            rom.ramEnable = val == 0x0a;
        }
    } else if (addr > 0x9fff && addr < 0xc000 && rom.ramAvail && rom.ramEnable) {
        addr &= 0x1ff;
        rom.ram[addr] = val;
    }
}

void cartInit(char *file) {
    rom.filenameSize = strlen(file);
    rom.filename = malloc(rom.filenameSize + 1);
    strncpy(rom.filename, file, rom.filenameSize);
    rom.filename[rom.filenameSize] = 0;

    FILE *f = fopen(rom.filename, "r");
    if (f == NULL) {
	printf("File Failed to Open\n");
	exit(1);
    }

    fseek(f, 0, SEEK_END);
    rom.cartSize = ftell(f);
    rewind(f);
    rom.cartridge = malloc(rom.cartSize);
    fread(rom.cartridge, rom.cartSize, 1, f);
    fclose(f);

    rom.header.entry = (uint8_t *)(rom.cartridge + 0x100);
    rom.header.logo = (uint8_t *)(rom.cartridge + 0x104);
    rom.header.title = (char *)(rom.cartridge + 0x134);

    rom.header.title[0xf] = 0;

    rom.header.newLicenseeCode = (*(rom.cartridge + 0x144) << 8) | (*(rom.cartridge + 0x145));
    rom.header.sgbFlag = *(rom.cartridge + 0x146);
    rom.header.type = *(rom.cartridge + 0x147);
    rom.header.romSize = *(rom.cartridge + 0x148);
    rom.header.ramSize = *(rom.cartridge + 0x149);
    rom.header.dest = *(rom.cartridge + 0x14a);
    rom.header.oldLicenseeCode = *(rom.cartridge + 0x14b);
    rom.header.version = *(rom.cartridge + 0x14c);
    rom.header.checksum = *(rom.cartridge + 0x14d);
    rom.header.globalChecksum = (*(rom.cartridge + 0x14e) << 8) | (*(rom.cartridge + 0x14f));

    displayInfo();
    cartTypeSelector();

    // Need to move these to the exit function When made
    free(rom.cartridge);
    free(rom.filename);
}

