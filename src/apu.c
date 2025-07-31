#include <stdio.h>
#include <stdbool.h>

#include "./include/apu.h"
#include "./include/gui.h"

#define FR_64_HZ 65535
#define FR_128_HZ 32767
#define FR_256_HZ 16383

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

uint8_t dutyWaves[0x4] = {
    0xfe, 0x7e, 0x78, 0x81
};

// Channel 1 Functions and variables
#define CH_1_PACE ((soundRegs[0x00] >> 4) & 0x3)
#define CH_1_DIR (soundRegs[0x00] & 0x08)
#define CH_1_STEP (soundRegs[0x00] & 0x07)
#define CH_1_DUTY (soundRegs[0x01] >> 6)
#define CH_1_VOL_REG (soundRegs[0x02])
#define CH_1_LEN_EN (soundRegs[0x04] & 0x40)
#define CH_1_TRIG (soundRegs[0x04] & 0x80)

uint16_t ch1Period = 0x0;
uint16_t ch1CurPeriod = 0x0;
uint16_t ch1CurPeriodVal = 0x0;
uint8_t ch1VolReg = 0xf3;
uint8_t ch1Vol = 0xf;
uint8_t ch1DutyIdx = 0;
uint32_t ch1VolClock = 0;
uint8_t ch1VolTimer = 0;
uint8_t ch1LenTimer = 0x0;
uint32_t ch1LenClock = 0;
uint8_t ch1CurPace = 0;
uint32_t ch1SweepClock = 0;
uint8_t ch1SweepTimer = 0;
bool sweepEn = true;

#define CH_1_CUR_VOL (ch1VolReg >> 4)
#define CH_1_CUR_ENV (ch1VolReg & 0x8)
#define CH_1_CUR_SWEEP (ch1VolReg & 0x7)

static void ch1Reset() {
    // Reseting internal registers
    ch1Period = 0x0;
    ch1CurPeriod = 0x0;
    ch1CurPeriodVal = 0x0;
    ch1VolReg = 0x0;
    ch1Vol = 0x0;
    ch1DutyIdx = 0;
    ch1VolClock = 0;
    ch1VolTimer = 0;
    ch1LenTimer = 0x3f;
    ch1LenClock = 0;
    ch1CurPace = 0;
    ch1SweepClock = 0;
    ch1SweepTimer = 0;
    sweepEn = false;

    // Reseting actual registers
    soundRegs[0x00] &= 0x80;
    soundRegs[0x01] &= 0x3f;
    soundRegs[0x02] = 0;
    soundRegs[0x04] &= 0xbf;
}

static uint16_t ch1CalcNewPeriod() {
    uint16_t curPeriod = ch1CurPeriod;
    uint16_t deltaVal = curPeriod >> CH_1_STEP;

    if (CH_1_DIR) {
        curPeriod -= deltaVal;
    } else {
        curPeriod += deltaVal;

        if (curPeriod > 0x7ff) {
            // Disable the Channel 1 here
            soundRegs[0x16] &= (~0x01);
        }
    }

    // This assumes that if there is an underflow it would also be greater than
    // 0x7ff, since it is a unsigned number
    if (curPeriod > 0x7ff) {
        curPeriod = ch1CurPeriod;
    }

    return curPeriod;
}

static void ch1Trigger() {
    ch1CurPeriod = ch1Period;
    ch1CurPeriodVal = ch1CurPeriod;
    ch1DutyIdx = 0;

    ch1VolReg = CH_1_VOL_REG;
    ch1VolClock = 0;
    ch1VolTimer = 0;
    ch1Vol = CH_1_CUR_VOL;
    if (ch1VolReg & 0xf8) {
        soundRegs[0x16] |= 0x01;
    }

    // Skeptical on whether this goes back to zero or the last written length
    // value, might have to do some testing
    if (ch1LenTimer > 0x3f) {
        ch1LenClock = 0;
        ch1LenTimer = 0;
    }

    ch1SweepClock = 0;
    ch1SweepTimer = 0;
    ch1CurPace = CH_1_PACE;
    sweepEn = ch1CurPace + CH_1_STEP > 0;
    if (CH_1_STEP) {
        ch1CalcNewPeriod();
    }
}

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
            ch1CurPeriod = ch1Period;
        }
        ch1CurPeriodVal = ch1CurPeriod;
    }

    if (CH_1_CUR_SWEEP) {
        ch1VolClock += cycles;
        if (ch1VolClock > FR_64_HZ) {
            ch1VolClock -= (FR_64_HZ + 1);
            ch1VolTimer++;
            if (ch1VolTimer >= CH_1_CUR_SWEEP) {
                ch1VolTimer = 0;
                if (CH_1_CUR_ENV) {
                    if (ch1Vol < 0xf) {
                        ch1Vol++;
                    }
                } else {
                    if (ch1Vol > 0) {
                        ch1Vol--;
                    }
                }
            }
        }
    }

    if (CH_1_LEN_EN) {
        ch1LenClock += cycles;
        if (ch1LenClock > FR_256_HZ) {
            ch1LenClock -= (FR_256_HZ + 1);
            ch1LenTimer++;
            if (ch1LenTimer >= 0x3f) {
                // Disable the Channel 1 here
                soundRegs[0x16] &= (~0x01);
            }
        }
    }

    ch1SweepClock += cycles;
    if (ch1SweepClock > FR_128_HZ) {
        ch1SweepClock -= (FR_128_HZ + 1);
        if (ch1CurPace > 0 && sweepEn) {
            ch1SweepTimer++;
            if (ch1SweepTimer >= ch1CurPace) {
                ch1SweepTimer = 0;
                ch1CurPace = CH_1_PACE;
                ch1Period = ch1CalcNewPeriod();
                ch1CurPeriod = ch1Period;
                ch1CalcNewPeriod();
            }
        }
    }
}

// Channel 2 Functions and variables
#define CH_2_DUTY (soundRegs[0x06] >> 6)
#define CH_2_VOL_REG (soundRegs[0x07])
#define CH_2_PERIOD (soundRegs[0x08] | ((soundRegs[0x09] & 0x7) << 8))
#define CH_2_LEN_EN (soundRegs[0x09] & 0x40)
#define CH_2_TRIG (soundRegs[0x09] & 0x80)

uint16_t ch2Period = 0x7ff;
uint16_t ch2CurPeriod = 0x7ff;
uint16_t ch2CurPeriodVal = 0x7ff;
uint8_t ch2VolReg = 0x00;
uint8_t ch2Vol = 0x0;
uint8_t ch2DutyIdx = 0;
uint32_t ch2VolClock = 0;
uint8_t ch2VolTimer = 0;
uint8_t ch2LenTimer = 0x3f;
uint32_t ch2LenClock = 0;

#define CH_2_CUR_VOL (ch2VolReg >> 4)
#define CH_2_CUR_ENV (ch2VolReg & 0x8)
#define CH_2_CUR_SWEEP (ch2VolReg & 0x7)

static void ch2Reset() {
    // Reseting internal registers
    ch2Period = 0x0;
    ch2CurPeriod = 0x0;
    ch2CurPeriodVal = 0x0;
    ch2VolReg = 0x00;
    ch2Vol = 0x0;
    ch2DutyIdx = 0;
    ch2VolClock = 0;
    ch2VolTimer = 0;
    ch2LenTimer = 0x3f;
    ch2LenClock = 0;

    // Reseting external registers
    soundRegs[0x06] &= 0x3f;
    soundRegs[0x07] = 0;
    soundRegs[0x09] &= 0xbf;
}

static void ch2Trigger() {
    ch2CurPeriod = ch2Period;
    ch2CurPeriodVal = ch2CurPeriod;
    ch2DutyIdx = 0;

    ch2VolReg = CH_2_VOL_REG;
    ch2VolClock = 0;
    ch2VolTimer = 0;
    ch2Vol = CH_2_CUR_VOL;
    if (ch2VolReg & 0xf8) {
        soundRegs[0x16] |= 0x02;
    }

    // Skeptical on whether this goes back to zero or the last written length
    // value, might have to do some testing
    if (ch2LenTimer > 0x3f) {
        ch2LenClock = 0;
        ch2LenTimer = 0;
    }
}

static void ch2Tick(uint32_t cycles) {
    uint32_t mClocks = cycles >> 2;
    ch2CurPeriodVal += mClocks;
    while (ch2CurPeriodVal > 0x7ff) {

        // Need to implement what the duty actually does here
        ch2DutyIdx++;

        if (ch2DutyIdx > 0x7) {
            ch2DutyIdx = 0;

            // This means that a sample is over so the period can refresh from 
            // the APU registers
            ch2CurPeriod = ch2Period;
        }
        ch2CurPeriodVal = ch2CurPeriod;
    }

    if (CH_2_CUR_SWEEP) {
        ch2VolClock += cycles;
        if (ch2VolClock > FR_64_HZ) {
            ch2VolClock -= (FR_64_HZ + 1);
            ch2VolTimer++;
            if (ch2VolTimer >= CH_2_CUR_SWEEP) {
                ch2VolTimer = 0;
                if (CH_2_CUR_ENV) {
                    if (ch2Vol < 0xf) {
                        ch2Vol++;
                    }
                } else {
                    if (ch2Vol > 0) {
                        ch2Vol--;
                    }
                }
            }
        }
    }

    if (CH_2_LEN_EN) {
        ch2LenClock += cycles;
        if (ch2LenClock > FR_256_HZ) {
            ch2LenClock -= (FR_256_HZ + 1);
            ch2LenTimer++;
            if (ch2LenTimer >= 0x3f) {
                // Disable the Channel 2 here
                soundRegs[0x16] &= (~0x02);
            }
        }
    }
}

// Channel 3 Functions and variables
#define CH_3_DAC (soundRegs[0x0a] & 0x80)
#define CH_3_LEN (soundRegs[0x0b])
#define CH_3_OUT_LV ((soundRegs[0x0c] >> 5) & 0x3)
#define CH_3_LEN_EN (soundRegs[0x0e] & 0x40)
#define CH_3_TRIG (soundRegs[0x0e] & 0x80)

uint16_t ch3Period = 0x7ff;
uint16_t ch3CurPeriod = 0x7ff;
uint16_t ch3CurPeriodVal = 0x7ff;
uint16_t ch3LenTimer = 0xff;
uint32_t ch3LenClock = 0;
uint8_t waveRamIdx = 0;

static void ch3Reset() {
    // Reseting internal Registers
    ch3Period = 0x0;
    ch3CurPeriod = 0x0;
    ch3CurPeriodVal = 0x0;
    ch3LenTimer = 0xff;
    ch3LenClock = 0;
    waveRamIdx = 0;

    soundRegs[0x0a] &= 0x7f;
    soundRegs[0x0c] &= 0x9f;
    soundRegs[0x0e] &= 0xbf;
}

static void ch3Trigger() {
    ch3CurPeriod = ch3Period;
    ch3CurPeriodVal = ch3CurPeriod;
    waveRamIdx = 0;

    if (CH_3_DAC) {
        soundRegs[0x16] |= 0x08;
    }

    // Need to check if volume needs to be retriggered or just changed on write

    if (ch3LenTimer > 0xff) {
        ch3LenTimer = 0;
        ch3LenClock = 0;
    }
}

static void ch3Tick(uint32_t cycles) {
    uint32_t dotClock = cycles >> 1;
    ch3CurPeriodVal += dotClock;
    
    while (ch3CurPeriodVal > 0x7ff) {

        waveRamIdx++;

        if (waveRamIdx > 0x1f) {
            waveRamIdx = 0;

            ch3CurPeriod = ch3Period;
        }
        ch3CurPeriodVal = ch3CurPeriod;
    }

    if (CH_3_LEN_EN) {
        ch3LenClock += cycles;
        if (ch3LenClock > FR_256_HZ) {
            ch3LenClock -= (FR_256_HZ + 1);
            ch3LenTimer++;
            if (ch3LenTimer >= 0xff) {
                // Disable the Channel 3 here
                soundRegs[0x16] &= (~0x04);
            }
        }
    }
}

// Channel 4 Functions and variables
#define CH_4_VOL_REG (soundRegs[0x11])
#define CH_4_SHIFT (soundRegs[0x12] >> 0x04)
#define CH_4_LSFR (soundRegs[0x12] & 0x08)
#define CH_4_DIVIDER (soundRegs[0x12] & 0x07)
#define CH_4_LEN_EN (soundRegs[0x13] & 0x40)
#define CH_4_TRIG (soundRegs[0x13] & 0x80)

uint8_t ch4VolReg = 0x00;
uint8_t ch4Vol = 0;
uint8_t ch4VolTimer = 0;
uint32_t ch4VolClock = 0;
uint8_t ch4LenTimer = 0x3f;
uint32_t ch4LenClock = 0;
uint32_t ch4Clock = 0;
uint32_t ch4FreqCycles = 0;

uint16_t ch4LSFR = 0x7fff;

#define CH_4_CUR_VOL (ch4VolReg >> 4)
#define CH_4_CUR_ENV (ch4VolReg & 0x8)
#define CH_4_CUR_SWEEP (ch4VolReg & 0x7)

static void ch4Reset() {
    ch4VolReg = 0x00;
    ch4Vol = 0;
    ch4VolTimer = 0;
    ch4VolClock = 0;
    ch4LenTimer = 0x3f;
    ch4LenClock = 0;
    ch4Clock = 0;
    ch4FreqCycles = 0;
    ch4LSFR = 0;

    soundRegs[0x11] = 0;
    soundRegs[0x12] = 0;
    soundRegs[0x13] &= 0xbf;
}

static void ch4Trigger() {
    ch4VolReg = CH_4_VOL_REG;
    ch4VolClock = 0;
    ch4VolTimer = 0;
    ch4Vol = CH_4_CUR_VOL;
    if (ch4VolReg & 0xf8) {
        soundRegs[0x16] |= 0x08;
    }
    //
    // Skeptical on whether this goes back to zero or the last written length
    // value, might have to do some testing
    if (ch4LenTimer > 0x3f) {
        ch4LenClock = 0;
        ch4LenTimer = 0;
    }

    ch4LSFR = 0x7fff;
}

static void ch4Tick(uint32_t cycles) {
    ch4Clock += cycles;
    if (ch4Clock > ch4FreqCycles) {
        ch4Clock = 0;

        uint16_t bitSwitch = (ch4LSFR ^ (ch4LSFR >> 1)) & 0x1;

        ch4LSFR &= ~0x8000;
        ch4LSFR |= bitSwitch << 15;

        if (CH_4_LSFR) {
            ch4LSFR &= ~0x80;
            ch4LSFR |= bitSwitch << 7;
        }

        ch4LSFR >>= 1;

        if (!CH_4_DIVIDER) {
            ch4FreqCycles = 8;
        } else {
            ch4FreqCycles = 16 * CH_4_DIVIDER;
        }
        ch4FreqCycles <<= CH_4_SHIFT;
    }

    if (CH_4_CUR_SWEEP) {
        ch4VolClock += cycles;
        if (ch4VolClock > FR_64_HZ) {
            ch4VolClock -= (FR_64_HZ + 1);
            ch4VolTimer++;
            if (ch4VolTimer >= CH_4_CUR_SWEEP) {
                ch4VolTimer = 0;
                if (CH_4_CUR_ENV) {
                    if (ch4Vol < 0xf) {
                        ch4Vol++;
                    }
                } else {
                    if (ch4Vol > 0) {
                        ch4Vol--;
                    }
                }
            }
        }
    }

    if (CH_4_LEN_EN) {
        ch4LenClock += cycles;
        if (ch4LenClock > FR_256_HZ) {
            ch4LenClock -= (FR_256_HZ + 1);
            ch4LenTimer++;
            if (ch4LenTimer >= 0x3f) {
                // Disable channel 4 here
                soundRegs[0x16] &= (~0x08);
            }
        }
    }
}

#define AUD_ON (soundRegs[0x16] & 0x80)
#define CH4_ON (soundRegs[0x16] & 0x08)
#define CH3_ON (soundRegs[0x16] & 0x04)
#define CH2_ON (soundRegs[0x16] & 0x02)
#define CH1_ON (soundRegs[0x16] & 0x01)

#define CH4_L_ON (soundRegs[0x15] & 0x80)
#define CH3_L_ON (soundRegs[0x15] & 0x40)
#define CH2_L_ON (soundRegs[0x15] & 0x20)
#define CH1_L_ON (soundRegs[0x15] & 0x10)

#define CH4_R_ON (soundRegs[0x15] & 0x08)
#define CH3_R_ON (soundRegs[0x15] & 0x04)
#define CH2_R_ON (soundRegs[0x15] & 0x02)
#define CH1_R_ON (soundRegs[0x15] & 0x01)

// Don't know what VIN values are supposed to do, so I'm just ignoring them for
// now
#define L_VOL ((soundRegs[0x14] >> 4) & 0x7)
#define R_VOL (soundRegs[0x14] & 0x7)

uint32_t sampleTimer = 0;

void incApuTimer(uint32_t cycles) {
    sampleTimer += cycles;
    if (sampleTimer >= CLOCKS_PER_SAMPLE) {
        sampleTimer = 0;
        uint8_t ch1Sample = 0;
        uint8_t ch2Sample = 0;
        uint8_t ch3Sample = 0;
        uint8_t ch4Sample = 0;

        if (CH1_ON) {
            if (dutyWaves[CH_1_DUTY] & (1 << (7 - ch1DutyIdx))) {
                ch1Sample = ch1Vol;
            }
        }

        if (CH2_ON) {
            if (dutyWaves[CH_2_DUTY] & (1 << (7 - ch2DutyIdx))) {
                ch2Sample = ch2Vol;
            }
        }

        if (CH3_ON && CH_3_OUT_LV) {
            ch3Sample = soundRegs[0x20 + (waveRamIdx >> 1)];
            if (!(waveRamIdx & 0x01)) {
                ch3Sample >>= 4;
            }
            ch3Sample &= 0xf;
        }

        if (CH4_ON) {
            if (ch4LSFR & 0x01) {
                ch4Sample = ch4Vol;
            }
        }

        pushAudio(
            soundRegs[0x15],
            soundRegs[0x14],
            ch1Sample,
            ch2Sample,
            ch3Sample,
            ch4Sample
        );
    }

    if (!AUD_ON) return;

    if (CH1_ON) {
        ch1Tick(cycles);
    }
    if (CH2_ON) {
        ch2Tick(cycles);
    }
    if (CH3_ON) {
        ch3Tick(cycles);
    }
    if (CH4_ON) {
        ch4Tick(cycles);
    }

}

uint8_t readSound(uint16_t addr) {
    if (addr > 0xff2f && addr < 0xff40 && CH3_ON) {
        return 0xff;
    }
    return soundRegs[addr - 0xff10];
}

void writeSound(uint16_t addr, uint8_t val) {
    uint16_t relAddr = addr - 0xff10;
    if (relAddr == 0x16) {
        if ((soundRegs[relAddr] & 0x80) > (val & 0x80)) {
            
            soundRegs[0x16] &= 0x70;
            soundRegs[0x15] = 0;
            soundRegs[0x14] = 0;

            ch1Reset();
            ch2Reset();
            ch3Reset();
            ch4Reset();
        }
        soundRegs[0x16] = (val & 0x80) | (soundRegs[0x16] & 0x7f);

        return;
    }

    if (relAddr > 0x1f && relAddr < 0x30) {
        if (!CH3_ON) {
            soundRegs[relAddr] = val;
        }

        return;
    }

    if (AUD_ON) {
        switch(relAddr) {
            case 0x12:
            case 0x14:
            case 0x15:
                soundRegs[relAddr] = val;
                break;
            case 0x00:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0x80) | (val & 0x7f);
                break;
            case 0x01:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0x3f) | (val & 0xc0);
                ch1LenTimer = val & 0x3f;
                break;
            case 0x02:
                if (!(val & 0xf8)) {
                    soundRegs[0x16] &= (~0x01);
                }
                soundRegs[relAddr] = val;
                break;
            case 0x03:
                ch1Period = (ch1Period & 0x700) | (val);
                break;
            case 0x04:
                ch1Period = (ch1Period & 0xff) | ((val & 0x7) << 8);
                soundRegs[relAddr] = (soundRegs[relAddr] & 0xbf) | (val & 0x40);
                if (val & 0x80) {
                    ch1Trigger();
                }
                break;
            case 0x06:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0x3f) | (val & 0xc0);
                ch2LenTimer = val & 0x3f;
                break;
            case 0x07:
                if (!(val & 0xf8)) {
                    soundRegs[0x16] &= (~0x02);
                }
                soundRegs[relAddr] = val;
                break;
            case 0x08:
                ch2Period = (ch2Period & 0x700) | (val);
                break;
            case 0x09:
                ch2Period = (ch2Period & 0xff) | ((val & 0x7) << 8);
                soundRegs[relAddr] = (soundRegs[relAddr] & 0xbf) | (val & 0x40);
                if (val & 0x80) {
                    ch2Trigger();
                }
                break;
            case 0x0a:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0x7f) | (val & 0x80);
                if (!(val & 0x80)) {
                    soundRegs[0x16] &= (~0x04);
                }
                break;
            case 0x0b:
                ch3LenTimer = val;
                break;
            case 0x0c:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0x9f) | (val & 0x60);
                break;
            case 0x0d:
                ch3Period = (ch3Period & 0x700) | val;
                break;
            case 0x0e:
                ch3Period = (ch3Period & 0xff) | ((val & 0x7) << 8);
                soundRegs[relAddr] = (soundRegs[relAddr] & 0xbf) | (val & 0x40);
                if (val & 0x80) {
                    ch3Trigger();
                }
                break;
            case 0x10:
                ch4LenTimer = (val & 0x3f);
                break;
            case 0x11:
                if (!(val & 0xf8)) {
                    soundRegs[0x16] &= (~0x08);
                }
                soundRegs[relAddr] = val;
                break;
            case 0x13:
                soundRegs[relAddr] = (soundRegs[relAddr] & 0xbf) | (val & 0x40);
                if (val & 0x80) {
                    ch4Trigger();
                }
                break;
        }
    }
}

