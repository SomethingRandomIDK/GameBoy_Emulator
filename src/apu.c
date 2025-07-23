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

// Channel 1 Functions and variables
#define CH_1_PACE ((soundRegs[0x00] >> 4) & 0x3)
#define CH_1_DIR (soundRegs[0x00] & 0x08)
#define CH_1_STEP (soundRegs[0x00] & 0x07)
#define CH_1_DUTY (soundRegs[0x01] >> 6)
#define CH_1_LEN (soundRegs[0x01] & 0x3f)
#define CH_1_VOL (soundRegs[0x02] >> 4)
#define CH_1_ENV (soundRegs[0x02] & 0x8)
#define CH_1_SWEEP (soundRegs[0x02] & 0x7)
#define CH_1_PERIOD (soundRegs[0x03] | ((soundRegs[0x04] & 0x7) << 8))
#define CH_1_LEN_EN (soundRegs[0x04] & 0x40)
#define CH_1_TRIG (soundRegs[0x04] & 0x80)

uint8_t ch1CurPace = 0;
uint16_t ch1CurPeriod = 0x7ff;
uint16_t ch1CurPeriodVal = 0x7ff;
uint8_t ch1VolReg = 0xf3;
uint8_t ch1DutyIdx = 0;

static void ch1Tick(uint32_t cycles) {
    uint32_t mClocks = cycles >> 2;
    ch1CurPeriodVal += mClocks;
    while (ch1CurPeriodVal > 0x7ff) {

        // Need to implement what the duty actually does here
        ch1DutyIdx++;

        if (ch1DutyIdx > 0x7) {
            ch1DutyIdx = 0;

            // This means that a sample is over so the period can refresh from 
            // the APU registers
            ch1CurPeriod = CH_1_PERIOD;
        }
        ch1CurPeriodVal = ch1CurPeriod;
    }
}

// Channel 2 Functions and variables
#define CH_2_DUTY (soundRegs[0x06] >> 6)
#define CH_2_LEN (soundRegs[0x06] & 0x3f)
#define CH_2_VOL (soundRegs[0x07] >> 4)
#define CH_2_ENV (soundRegs[0x07] & 0x8)
#define CH_2_SWEEP (soundRegs[0x07] & 0x7)
#define CH_2_PERIOD (soundRegs[0x08] | ((soundRegs[0x09] & 0x7) << 8))
#define CH_2_LEN_EN (soundRegs[0x09] & 0x40)
#define CH_2_TRIG (soundRegs[0x09] & 0x80)

uint16_t ch2CurPeriod = 0x7ff;
uint8_t ch2VolReg = 0x00;

static void ch2Tick(uint32_t cycles) {
    return;
}

// Channel 3 Functions and variables
#define CH_3_DAC (soundRegs[0x0a] & 0x80)
#define CH_3_LEN (soundRegs[0x0b])
#define CH_3_OUT_LV ((soundRegs[0x0c] >> 5) & 0x3)
#define CH_3_PERIOD (soundRegs[0x0d] | ((soundRegs[0x0e] & 0x07) << 8))
#define CH_3_LEN_EN (soundRegs[0x0e] & 0x40)
#define CH_3_TRIG (soundRegs[0x0e] & 0x80)

uint16_t ch3CurPeriod = 0x7ff;

static void ch3Tick(uint32_t cycles) {
    return;
}

// Channel 4 Functions and variables
#define CH_4_LEN (soundRegs[0x10] & 0x3f)
#define CH_4_VOL (soundRegs[0x11] >> 0x04)
#define CH_4_ENV (soundRegs[0x11] & 0x08)
#define CH_4_SWEEP (soundRegs[0x11] & 0x07)
#define CH_4_SHIFT (soundRegs[0x12] >> 0x04)
#define CH_4_LSFR (soundRegs[0x12] & 0x08)
#define CH_4_DIVIDER (soundRegs[0x12] & 0x07)
#define CH_4_LEN_EN (soundRegs[0x13] & 0x40)
#define CH_4_TRIG (soundRegs[0x13] & 0x80)

uint8_t ch4VolReg = 0x00;

static void ch4Tick(uint32_t cycles) {
    return;
}

void incApuTimer(uint32_t cycles) {
    return;
}

uint8_t readSound(uint16_t addr) {
    return soundRegs[addr - 0xff10];
}

void writeSound(uint16_t addr, uint8_t val) {
    uint16_t relAddr = addr - 0xff10;
    switch(relAddr) {
        case 0x16:
            if ((soundRegs[0x16] & 0x80) > (val & 0x80)) {
                // TODO Turing the APU off
            } else if ((soundRegs[0x16] & 0x80) < (val & 0x80)) {
                // TODO Turing the APU on
            } else {
                soundRegs[0x16] = (val & 0xf0) | (soundRegs[0x16] & 0x0f);
            }
            break;
        default:
            soundRegs[relAddr] = val;
    }
}

