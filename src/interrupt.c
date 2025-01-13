#include "include/interrupt.h"
#include "include/bus.h"

uint8_t interFlags = 0xe1;
uint8_t interEnable = 0x0;

void raiseInterrupt(inter_t inter) {
    // 0xff0f is the interrupt flags, at this location the bit values show what
    // interrupts have been requested
    interFlags |= (1 << inter);
}

void handleInterrupt(gb_t *cpu) {
    uint8_t interCheck = interFlags & interEnable & 0x1f;

    if (cpu->interrupts && !!(interCheck)) {
        if (interCheck & 0x1) {
            // VBLANK
            //Resetting flags
            interFlags &= 0xfe;
            cpu->interrupts = false;
            cpu->halted = false;

            // Calling
            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x40;
        } else if (interCheck & 0x2) {
            // LCD
            interFlags &= 0xfd;
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x48;
        } else if (interCheck & 0x4) {
            // TIMER
            interFlags &= 0xfb;
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x50;
        } else if (interCheck & 0x8) {
            // SERIAL
            interFlags &= 0xf7;
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x58;
        } else if (interCheck & 0x10) {
            // JOYPAD
            interFlags &= 0xef;
            cpu->interrupts = false;
            cpu->halted = false;

            cpu->regs.sp -= 2;
            busWrite16(cpu->regs.sp, cpu->regs.pc);
            cpu->regs.pc = 0x60;
        }
    }
}

