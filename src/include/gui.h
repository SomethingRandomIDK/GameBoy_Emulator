#ifndef __GUI_H__
#define __GUI_H__

#include <stdint.h>
#include <stdbool.h>

void initGUI();
void closeGUI();

void frameDelay();
void renderFrame(uint8_t screen[144][160]);

#endif

