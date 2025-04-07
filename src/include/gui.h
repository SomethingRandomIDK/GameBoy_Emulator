#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

uint8_t readJoypad();
void writeJoypad(uint8_t val);

bool checkStop();

void initGUI();
void closeGUI();

void frameDelay();
void renderFrame(uint8_t screen[144][160]);

#endif

