#include "include/interrupt.h"
#include "include/bus.h"

void raiseInterrupt(inter_t inter) {
    // 0xff0f is the interrupt flags, at this location the bit values show what
    // interrupts have been requested
    uint8_t interFlags = busRead8(0xff0f);
    busWrite8(0xff0f, (interFlags | (1 << inter)));
}

void handleInterrupt(gb_t *cpu) {
    uint8_t interFlags = busRead8(0xff0f);
    uint8_t interEnable = busRead8(0xffff);
    uint8_t interCheck = interFlags & interEnable & 0x1f;

    if (cpu->interrupts && !!(interCheck)) {
        if (interCheck & 0x1) {
            // VBLANK
            //Resetting flags
            busWrite16(0xff0f, interFlags & 0xfe);
            cpu->interrupts = false;
            cpu->halted = false;
            // Calling
            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x40;
        } else if (interCheck & 0x2) {
            // LCD
            busWrite16(0xff0f, interFlags & 0xfd);
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x48;
        } else if (interCheck & 0x4) {
            // TIMER
            busWrite16(0xff0f, interFlags & 0xfb);
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x50;
        } else if (interCheck & 0x8) {
            // SERIAL
            busWrite16(0xff0f, interFlags & 0xf7);
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x58;
        } else if (interCheck & 0x10) {
            // JOYPAD
            busWrite16(0xff0f, interFlags & 0xef);
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x60;
        }
    }
}

