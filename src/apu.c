#include "./include/apu.h"

// $FF10	NR10	Sound channel 1 sweep	R/W	All
// $FF11	NR11	Sound channel 1 length timer & duty cycle	Mixed	All
// $FF12	NR12	Sound channel 1 volume & envelope	R/W	All
// $FF13	NR13	Sound channel 1 period low	W	All
// $FF14	NR14	Sound channel 1 period high & control	Mixed	All
// $FF16	NR21	Sound channel 2 length timer & duty cycle	Mixed	All
// $FF17	NR22	Sound channel 2 volume & envelope	R/W	All
// $FF18	NR23	Sound channel 2 period low	W	All
// $FF19	NR24	Sound channel 2 period high & control	Mixed	All
// $FF1A	NR30	Sound channel 3 DAC enable	R/W	All
// $FF1B	NR31	Sound channel 3 length timer	W	All
// $FF1C	NR32	Sound channel 3 output level	R/W	All
// $FF1D	NR33	Sound channel 3 period low	W	All
// $FF1E	NR34	Sound channel 3 period high & control	Mixed	All
// $FF20	NR41	Sound channel 4 length timer	W	All
// $FF21	NR42	Sound channel 4 volume & envelope	R/W	All
// $FF22	NR43	Sound channel 4 frequency & randomness	R/W	All
// $FF23	NR44	Sound channel 4 control	Mixed	All
// $FF24	NR50	Master volume & VIN panning	R/W	All
// $FF25	NR51	Sound panning	R/W	All
// $FF26	NR52	Sound on/off	Mixed	All
// $FF30-FF3F	Wave RAM	Storage for one of the sound channels’ waveform	R/W	All

uint8_t soundRegs[0x30] = {
    0x80, 0xbf, 0xf3, 0xff, 0xbf, 0xff, 0x3f, 0x00,
    0xff, 0xbf, 0x7f, 0xff, 0x9f, 0xff, 0xbf, 0xff,
    0xff, 0x00, 0x00, 0xbf, 0x77, 0xf3, 0xf1, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

void incApuTimer(uint32_t cycles) {
    return;
}

uint8_t readSound(uint16_t addr) {
    return soundRegs[addr - 0xff10];
}

void writeSound(uint16_t addr, uint8_t val) {
    switch(addr) {
        case 0xff26:
            break;
        default:
            soundRegs[addr - 0xff10] = val;
    }
}

