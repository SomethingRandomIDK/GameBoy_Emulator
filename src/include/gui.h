#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

uint8_t readJoypad();
void writeJoypad(uint8_t val);

bool checkStop();

void initGUI();
void closeGUI();

void incEventTimer(uint32_t cycles);

void renderFrame(uint8_t screen[144][160]);

void pushAudio(uint8_t sPanning, uint8_t mVol, uint8_t ch1, uint8_t ch2, uint8_t ch3, uint8_t ch4);

#define SAMPLE_SIZE 1024
#define GUI_FREQUENCY 44100

// This was found by gameboy CPU clock speed (4194304) / GUI_FREQUENCY
#define CLOCKS_PER_SAMPLE 95

#endif

