#include <stdio.h>
#include <stdlib.h>

#include "./logging/log.h"
#include "./include/interpreter.h"
#include "./include/bus.h"
#include "./include/cpu.h"
#include "./include/timer.h"

typedef void (*inst)(gb_t *);

static uint32_t numCycles = 0;

// NOP

static void nop(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc ++;
}

// 8-BIT LD

// Immediate 8 bits to register

static void ld_reg_n(uint8_t *val, gb_t *cpu) {
    *val = busRead8(++cpu->regs.pc);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.a, cpu);
}

static void ld_b_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.b, cpu);
}

static void ld_c_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.c, cpu);
}

static void ld_d_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.d, cpu);
}

static void ld_e_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.e, cpu);
}

static void ld_h_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.h, cpu);
}

static void ld_l_n(gb_t *cpu) {
    ld_reg_n(&cpu->regs.l, cpu);
}

static void ld_hl_n(gb_t *cpu) {
    incAllTimers(4);
    busWrite8(regHL(), busRead8(++cpu->regs.pc));
    numCycles = 8;
    cpu->regs.pc++;
}

// Register to Register

static void ld_b_b(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_c(gb_t *cpu) {
    cpu->regs.b = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_d(gb_t *cpu) {
    cpu->regs.b = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_e(gb_t *cpu) {
    cpu->regs.b = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_h(gb_t *cpu) {
    cpu->regs.b = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_l(gb_t *cpu) {
    cpu->regs.b = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_b_hl(gb_t *cpu) {
    cpu->regs.b = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_b_a(gb_t *cpu) {
    cpu->regs.b = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_b(gb_t *cpu) {
    cpu->regs.c = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_c(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_d(gb_t *cpu) {
    cpu->regs.c = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_e(gb_t *cpu) {
    cpu->regs.c = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_h(gb_t *cpu) {
    cpu->regs.c = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_l(gb_t *cpu) {
    cpu->regs.c = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_c_hl(gb_t *cpu) {
    cpu->regs.c = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_c_a(gb_t *cpu) {
    cpu->regs.c = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_b(gb_t *cpu) {
    cpu->regs.d = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_c(gb_t *cpu) {
    cpu->regs.d = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_d(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_e(gb_t *cpu) {
    cpu->regs.d = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_h(gb_t *cpu) {
    cpu->regs.d = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_l(gb_t *cpu) {
    cpu->regs.d = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_d_hl(gb_t *cpu) {
    cpu->regs.d = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_d_a(gb_t *cpu) {
    cpu->regs.d = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_b(gb_t *cpu) {
    cpu->regs.e = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_c(gb_t *cpu) {
    cpu->regs.e = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_d(gb_t *cpu) {
    cpu->regs.e = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_e(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_h(gb_t *cpu) {
    cpu->regs.e = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_l(gb_t *cpu) {
    cpu->regs.e = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_e_hl(gb_t *cpu) {
    cpu->regs.e = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_e_a(gb_t *cpu) {
    cpu->regs.e = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_b(gb_t *cpu) {
    cpu->regs.h = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_c(gb_t *cpu) {
    cpu->regs.h = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_d(gb_t *cpu) {
    cpu->regs.h = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_e(gb_t *cpu) {
    cpu->regs.h = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_h(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_l(gb_t *cpu) {
    cpu->regs.h = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_h_hl(gb_t *cpu) {
    cpu->regs.h = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_h_a(gb_t *cpu) {
    cpu->regs.h = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_b(gb_t *cpu) {
    cpu->regs.l = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_c(gb_t *cpu) {
    cpu->regs.l = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_d(gb_t *cpu) {
    cpu->regs.l = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_e(gb_t *cpu) {
    cpu->regs.l = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_h(gb_t *cpu) {
    cpu->regs.l = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_l(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_l_hl(gb_t *cpu) {
    cpu->regs.l = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_l_a(gb_t *cpu) {
    cpu->regs.l = cpu->regs.a;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_hl_b(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.b);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_c(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.c);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_d(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.d);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_e(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.e);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_h(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.h);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_l(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.l);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_b(gb_t *cpu) {
    cpu->regs.a = cpu->regs.b;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_c(gb_t *cpu) {
    cpu->regs.a = cpu->regs.c;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_d(gb_t *cpu) {
    cpu->regs.a = cpu->regs.d;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_e(gb_t *cpu) {
    cpu->regs.a = cpu->regs.e;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_h(gb_t *cpu) {
    cpu->regs.a = cpu->regs.h;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_l(gb_t *cpu) {
    cpu->regs.a = cpu->regs.l;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_a_hl(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_a(gb_t *cpu) {
    numCycles = 4;
    cpu->regs.pc++;
}

// Load value into A

static void ld_a_bc(gb_t *cpu) {
    cpu->regs.a = busRead8(regBC());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_de(gb_t *cpu) {
    cpu->regs.a = busRead8(regDE());
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_nn(gb_t *cpu) {
    incAllTimers(8);
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc +=2;
    cpu->regs.a = busRead8(addr);
    numCycles = 8;
}

static void ld_a_hld(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    setHL(regHL() - 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_a_hli(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    setHL(regHL() + 1);
    numCycles = 8;
    cpu->regs.pc++;
}

// This saves C + 0xff00 into A instead of saving C into A
static void ld_a_addr_c(gb_t *cpu) {
    uint16_t addr = 0xff00 | cpu->regs.c;
    cpu->regs.a = busRead8(addr);
    numCycles = 8;
    cpu->regs.pc++;
}

// This saves n + 0xff00 into A instead of saving n into A
static void ldh_a_n(gb_t *cpu) {
    incAllTimers(4);
    uint16_t addr = 0xff00 | busRead8(++cpu->regs.pc);
    cpu->regs.a = busRead8(addr);
    numCycles = 8;
    cpu->regs.pc++;
}

// Load A into value

static void ld_bc_a(gb_t *cpu) {
    busWrite8(regBC(), cpu->regs.a);
    numCycles = 4;
    cpu->regs.pc++;
}

static void ld_de_a(gb_t *cpu) {
    busWrite8(regDE(), cpu->regs.a);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_nn_a(gb_t *cpu) {
    incAllTimers(8);
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc += 2;
    busWrite8(addr, cpu->regs.a);
    numCycles = 8;
}

static void ld_hld_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    setHL(regHL() - 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hli_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    setHL(regHL() + 1);
    numCycles = 8;
    cpu->regs.pc++;
}

// This saves A into C + 0xff00 instead of saving A into C
static void ld_addr_c_a(gb_t *cpu) {
    uint16_t addr = 0xff00 | cpu->regs.c;
    busWrite8(addr, cpu->regs.a);
    numCycles = 8;
    cpu->regs.pc++;
}

static void ldh_n_a(gb_t *cpu) {
    incAllTimers(4);
    uint16_t addr = 0xff00 | busRead8(++cpu->regs.pc);
    busWrite8(addr, cpu->regs.a);
    numCycles = 8;
    cpu->regs.pc++;
}

// 16-BIT LD

static void ld_bc_nn(gb_t *cpu) {
    setBC(busRead16(++cpu->regs.pc));
    numCycles = 12;
    cpu->regs.pc += 2;
}

static void ld_de_nn(gb_t *cpu) {
    setDE(busRead16(++cpu->regs.pc));
    numCycles = 12;
    cpu->regs.pc += 2;
}

static void ld_hl_nn(gb_t *cpu) {
    setHL(busRead16(++cpu->regs.pc));
    numCycles = 12;
    cpu->regs.pc += 2;
}

static void ld_sp_nn(gb_t *cpu) {
    cpu->regs.sp = busRead16(++cpu->regs.pc);
    numCycles = 12;
    cpu->regs.pc += 2;
}

static void ld_sp_hl(gb_t *cpu) {
    cpu->regs.sp = regHL();
    numCycles = 8;
    cpu->regs.pc++;
}

static void ld_hl_sp_n(gb_t *cpu) {
    int8_t arg = (int8_t)busRead8(++cpu->regs.pc);
    setHL(cpu->regs.sp + arg);
    cpu->regs.pc++;

    uint16_t temp = regHL() ^ cpu->regs.sp ^ arg;
    setZ(false);
    setN(false);
    // XORing everything is supposed to show where there is a carry
    setC(!!(temp & 0x100));
    setH(!!(temp & 0x10));
    numCycles = 12;
}

static void ld_nn_sp(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    busWrite16(addr, cpu->regs.sp);
    numCycles = 20;
    cpu->regs.pc += 2;
}

// All stack push instructions

static void push(uint16_t val, gb_t *cpu) {
    cpu->regs.sp -= 2;
    busWrite16(cpu->regs.sp, val);
    numCycles = 16;
    cpu->regs.pc ++;
}

static void push_af(gb_t *cpu){
    push(regAF(), cpu);
}

static void push_bc(gb_t *cpu){
    push(regBC(), cpu);
}

static void push_de(gb_t *cpu){
    push(regDE(), cpu);
}

static void push_hl(gb_t *cpu){
    push(regHL(), cpu);
}

// All stack pop operations

static uint16_t pop(gb_t *cpu){
    uint16_t val = busRead16(cpu->regs.sp);
    cpu->regs.sp += 2;
    numCycles = 12;
    cpu->regs.pc ++;
    return val;
}

static void pop_bc(gb_t *cpu){
    setBC(pop(cpu));
}

static void pop_de(gb_t *cpu){
    setDE(pop(cpu));
}

static void pop_hl(gb_t *cpu){
    setHL(pop(cpu));
}

static void pop_af(gb_t *cpu){
    setAF((pop(cpu) & 0xfff0));
}

// 8 Bit ALU

// ADD
 
static void add8(uint8_t val, gb_t *cpu) {
    uint8_t temp = cpu->regs.a;
    cpu->regs.a += val;
    temp ^= (cpu->regs.a ^ val);

    setN(false);
    setZ(cpu->regs.a == 0);
    setC(cpu->regs.a < val);
    // XORing everything is supposed to show where there is a carry
    // XORing two binary numbers gives the what the digit should be after
    // adding, not taking into account any carries

    // This should work in theory, might have to change it later
    // If this errors then replace with this line:
    // setH(((temp & 0xf) + (val & 0xf)) > 0xf);
    // (Can remove temp variable and perform the operations before the addition,
    // if this doesn't work)
    setH(!!(temp & 0x10));
    numCycles = 4;
}

static void add_a_b(gb_t *cpu) {
    add8(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void add_a_c(gb_t *cpu) {
    add8(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void add_a_d(gb_t *cpu) {
    add8(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void add_a_e(gb_t *cpu) {
    add8(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void add_a_h(gb_t *cpu) {
    add8(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void add_a_l(gb_t *cpu) {
    add8(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void add_a_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    add8(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void add_a_a(gb_t *cpu) {
    add8(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void add_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    add8(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// ADC

static void adc(uint8_t val, gb_t *cpu) {
    uint8_t temp = cpu->regs.a;
    cpu->regs.a += val + flagC();
    temp ^= cpu->regs.a ^ val;

    setN(false);
    setZ(cpu->regs.a == 0);
    setC((cpu->regs.a < val) || (flagC() && cpu->regs.a == val));
    setH(!!(temp & 0x10));
    numCycles = 4;
}

static void adc_a_b(gb_t *cpu) {
    adc(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void adc_a_c(gb_t *cpu) {
    adc(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void adc_a_d(gb_t *cpu) {
    adc(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void adc_a_e(gb_t *cpu) {
    adc(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void adc_a_h(gb_t *cpu) {
    adc(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void adc_a_l(gb_t *cpu) {
    adc(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void adc_a_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    adc(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void adc_a_a(gb_t *cpu) {
    adc(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void adc_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    adc(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// SUB

static void sub(uint8_t val, gb_t *cpu) {
    uint8_t temp = cpu->regs.a;
    cpu->regs.a -= val;
    setC(temp < val);

    // For digit wise binary subtraction the result is the same as the result
    // of and XOR calculation, so then XORing that with the answer should show 
    // the bits where all the borrows occured
    temp ^= cpu->regs.a ^ val;

    setN(true);
    setZ(cpu->regs.a == 0);
    setH(!!(temp & 0x10));
    numCycles = 4;
}

static void sub_b(gb_t *cpu) {
    sub(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void sub_c(gb_t *cpu) {
    sub(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void sub_d(gb_t *cpu) {
    sub(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void sub_e(gb_t *cpu) {
    sub(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void sub_h(gb_t *cpu) {
    sub(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void sub_l(gb_t *cpu) {
    sub(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void sub_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    sub(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void sub_a(gb_t *cpu) {
    sub(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void sub_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    sub(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// SBC

static void sbc(uint8_t val, gb_t *cpu) {
    uint8_t temp = cpu->regs.a;
    cpu->regs.a -= (val + flagC());

    setC(temp < (val + flagC()));

    temp ^= (cpu->regs.a ^ val);

    setN(true);
    setZ(cpu->regs.a == 0);
    // So same logic as last time with the Sub, but I'm less sure about this one
    // I feel like it should work, and the logic checks out, but I'm still 
    // unsure
    // Tested it in a notebook, so this should hold up
    setH(!!(temp & 0x10));
    numCycles = 4;
}

static void sbc_a_b(gb_t *cpu) {
    sbc(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void sbc_a_c(gb_t *cpu) {
    sbc(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void sbc_a_d(gb_t *cpu) {
    sbc(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void sbc_a_e(gb_t *cpu) {
    sbc(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void sbc_a_h(gb_t *cpu) {
    sbc(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void sbc_a_l(gb_t *cpu) {
    sbc(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void sbc_a_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    sbc(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void sbc_a_a(gb_t *cpu) {
    sbc(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void sbc_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    sbc(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// AND

static void and(uint8_t val, gb_t *cpu) {
    cpu->regs.a &= val;

    setN(false);
    setH(true);
    setC(false);
    setZ(cpu->regs.a == 0);
    numCycles = 4;
}

static void and_b(gb_t *cpu) {
    and(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void and_c(gb_t *cpu) {
    and(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void and_d(gb_t *cpu) {
    and(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void and_e(gb_t *cpu) {
    and(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void and_h(gb_t *cpu) {
    and(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void and_l(gb_t *cpu) {
    and(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void and_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    and(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void and_a(gb_t *cpu) {
    and(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void and_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    and(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// XOR

static void xor(uint8_t val, gb_t *cpu) {
    cpu->regs.a ^= val;
    setZ(cpu->regs.a == 0);
    setN(false);
    setH(false);
    setC(false);
    numCycles = 4;
}

static void xor_b(gb_t *cpu) {
    xor(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void xor_c(gb_t *cpu) {
    xor(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void xor_d(gb_t *cpu) {
    xor(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void xor_e(gb_t *cpu) {
    xor(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void xor_h(gb_t *cpu) {
    xor(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void xor_l(gb_t *cpu) {
    xor(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void xor_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    xor(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void xor_a(gb_t *cpu) {
    xor(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void xor_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    xor(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// OR

static void or(uint8_t val, gb_t *cpu) {
    cpu->regs.a |= val;

    setZ(cpu->regs.a == 0);
    setN(false);
    setH(false);
    setC(false);
    numCycles = 4;
}

static void or_b(gb_t *cpu) {
    or(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void or_c(gb_t *cpu) {
    or(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void or_d(gb_t *cpu) {
    or(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void or_e(gb_t *cpu) {
    or(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void or_h(gb_t *cpu) {
    or(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void or_l(gb_t *cpu) {
    or(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void or_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    or(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void or_a(gb_t *cpu) {
    or(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void or_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    or(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

//CP

static void cp(uint8_t val, gb_t *cpu){
    setZ(cpu->regs.a == val);
    setN(true);
    setH((cpu->regs.a & 0xf) < (val & 0xf));
    setC(cpu->regs.a < val);
    numCycles = 4;
}

static void cp_b(gb_t *cpu) {
    cp(cpu->regs.b, cpu);
    cpu->regs.pc++;
}

static void cp_c(gb_t *cpu) {
    cp(cpu->regs.c, cpu);
    cpu->regs.pc++;
}

static void cp_d(gb_t *cpu) {
    cp(cpu->regs.d, cpu);
    cpu->regs.pc++;
}

static void cp_e(gb_t *cpu) {
    cp(cpu->regs.e, cpu);
    cpu->regs.pc++;
}

static void cp_h(gb_t *cpu) {
    cp(cpu->regs.h, cpu);
    cpu->regs.pc++;
}

static void cp_l(gb_t *cpu) {
    cp(cpu->regs.l, cpu);
    cpu->regs.pc++;
}

static void cp_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    cp(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

static void cp_a(gb_t *cpu) {
    cp(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void cp_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    cp(val, cpu);
    numCycles = 8;
    cpu->regs.pc++;
}

// INC

static void inc(uint8_t *val, gb_t *cpu) {
    (*val)++;
    setZ(*val == 0);
    setN(false);
    setH((*val & 0xf) == 0);
    numCycles = 4;
    cpu->regs.pc++;
}

static void inc_b(gb_t *cpu) {
    inc(&cpu->regs.b, cpu);
}

static void inc_c(gb_t *cpu) {
    inc(&cpu->regs.c, cpu);
}

static void inc_d(gb_t *cpu) {
    inc(&cpu->regs.d, cpu);
}

static void inc_e(gb_t *cpu) {
    inc(&cpu->regs.e, cpu);
}

static void inc_h(gb_t *cpu) {
    inc(&cpu->regs.h, cpu);
}

static void inc_l(gb_t *cpu) {
    inc(&cpu->regs.l, cpu);
}

// Naming conflict so this one is different
static void inc_addr_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    inc(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
    numCycles = 8;
}

static void inc_a(gb_t *cpu) {
    inc(&cpu->regs.a, cpu);
}

// DEC

static void dec(uint8_t *val, gb_t *cpu) {
    (*val)--;
    setZ(*val == 0);
    setN(true);
    setH((*val & 0xf) == 0xf);
    numCycles = 4;
    cpu->regs.pc++;
}

static void dec_b(gb_t *cpu) {
    dec(&cpu->regs.b, cpu);
}

static void dec_c(gb_t *cpu) {
    dec(&cpu->regs.c, cpu);
}

static void dec_d(gb_t *cpu) {
    dec(&cpu->regs.d, cpu);
}

static void dec_e(gb_t *cpu) {
    dec(&cpu->regs.e, cpu);
}

static void dec_h(gb_t *cpu) {
    dec(&cpu->regs.h, cpu);
}

static void dec_l(gb_t *cpu) {
    dec(&cpu->regs.l, cpu);
}

// Changed to avoid naming conflict
static void dec_addr_hl(gb_t *cpu) {
    uint8_t val = busRead8(regHL());
    dec(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
    numCycles = 8;
}

static void dec_a(gb_t *cpu) {
    dec(&cpu->regs.a, cpu);
}

// Miscellaneous ALU intructions

// Decimal Adjust A

static void daa(gb_t *cpu) {
    if (!flagN()) {
        if (flagC() || cpu->regs.a > 0x99) {
            cpu->regs.a += 0x60;
            setC(true);
        }
        if (flagH() || (cpu->regs.a & 0xf) > 0x9) {
            cpu->regs.a += 0x6;
            setH(false);
        }
    } else {
        if (flagC() && flagH()) {
            cpu->regs.a += 0x9a;
            setH(false);
        } else if (flagC()) {
            cpu->regs.a += 0xa0;
        } else if (flagH()) {
            cpu->regs.a += 0xfa;
            setH(false);
        }
    }

    setZ(cpu->regs.a == 0);
    numCycles = 4;
    cpu->regs.pc++;
}

// Complement A register

static void cpl(gb_t *cpu) {
    cpu->regs.a ^= 0xff;
    setN(true);
    setH(true);
    numCycles = 4;
    cpu->regs.pc++;
}

// Complement The Carry Flag

static void ccf(gb_t *cpu) {
    setC(!flagC());
    setN(false);
    setH(false);
    numCycles = 4;
    cpu->regs.pc++;
}

// Set the Carry Flag

static void scf(gb_t *cpu) {
    setN(false);
    setH(false);
    setC(true);
    numCycles = 4;
    cpu->regs.pc++;
}

// 16 Bit ALU

// ADD HL

static void add_hl(uint16_t val, gb_t *cpu) {
    uint16_t temp = regHL();
    setHL(temp + val);
    temp ^= regHL() ^ val;

    // The zero flag is not affected for this command
    setN(false);
    setH(!!(temp &0x1000));
    setC(regHL() < val);

    numCycles = 8;
    cpu->regs.pc++;
}

static void add_hl_bc(gb_t *cpu) {
    add_hl(regBC(), cpu);
}

static void add_hl_de(gb_t *cpu) {
    add_hl(regDE(), cpu);
}

static void add_hl_hl(gb_t *cpu) {
    add_hl(regHL(), cpu);
}

static void add_hl_sp(gb_t *cpu) {
    add_hl(cpu->regs.sp, cpu);
}

//ADD SP n

static void add_sp_n(gb_t *cpu) {
    setZ(false);
    setN(false);
    incAllTimers(4);
    int8_t  val = (int8_t)busRead8(++cpu->regs.pc);
    uint16_t temp = cpu->regs.sp;
    cpu->regs.sp += val;
    cpu->regs.pc++;

    temp ^= cpu->regs.sp ^ val;

    setH(!!(temp & 0x10));
    setC(!!(temp & 0x100));
    numCycles = 12;
}

// 16 bit INC

static void inc_bc(gb_t *cpu) {
    setBC(regBC() + 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void inc_de(gb_t *cpu) {
    setDE(regDE() + 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void inc_hl(gb_t *cpu) {
    setHL(regHL() + 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void inc_sp(gb_t *cpu) {
    cpu->regs.sp++;
    numCycles = 8;
    cpu->regs.pc++;
}

// 16 Bit DEC

static void dec_bc(gb_t *cpu) {
    setBC(regBC() - 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void dec_de(gb_t *cpu) {
    setDE(regDE() - 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void dec_hl(gb_t *cpu) {
    setHL(regHL() - 1);
    numCycles = 8;
    cpu->regs.pc++;
}

static void dec_sp(gb_t *cpu) {
    cpu->regs.sp--;
    numCycles = 8;
    cpu->regs.pc++;
}

// JUMPS

// Jump to immediate 16 bits

static void jp_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    numCycles = 16;
    cpu->regs.pc = addr;
}

// Conditional Jumps

static void jp_nz_nn(gb_t *cpu) {
    if (!flagZ()) {
        jp_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void jp_z_nn(gb_t *cpu) {
    if (flagZ()) {
        jp_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void jp_nc_nn(gb_t *cpu) {
    if (!flagC()) {
        jp_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void jp_c_nn(gb_t *cpu) {
    if (flagC()) {
        jp_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

// Jump to address contained in HL

static void jp_hl(gb_t *cpu) {
    cpu->regs.pc = regHL();
    numCycles = 4;
}

// Relative Jumps

static void jr_n(gb_t *cpu) {
    int8_t jmpDiff = (int8_t)busRead8(++cpu->regs.pc);
    numCycles = 12;
    cpu->regs.pc++;
    cpu->regs.pc += jmpDiff;
}

// Relative Conditional Jumps

static void jr_nz_n(gb_t *cpu) {
    if (!flagZ()) {
        jr_n(cpu);
    } else {
        numCycles = 8;
        cpu->regs.pc += 2;
    }
}

static void jr_z_n(gb_t *cpu) {
    if (flagZ()) {
        jr_n(cpu);
    } else {
        numCycles = 8;
        cpu->regs.pc += 2;
    }
}

static void jr_nc_n(gb_t *cpu) {
    if (!flagC()) {
        jr_n(cpu);
    } else {
        numCycles = 8;
        cpu->regs.pc += 2;
    }
}

static void jr_c_n(gb_t *cpu) {
    if (flagC()) {
        jr_n(cpu);
    } else {
        numCycles = 8;
        cpu->regs.pc += 2;
    }
}

// CALLS

static void call_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    push(cpu->regs.pc + 2, cpu);
    numCycles = 24;
    cpu->regs.pc = addr;
}

// Conditional Calls

static void call_nz_nn(gb_t *cpu) {
    if (!flagZ()) {
        call_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void call_z_nn(gb_t *cpu) {
    if (flagZ()) {
        call_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void call_nc_nn(gb_t *cpu) {
    if (!flagC()) {
        call_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

static void call_c_nn(gb_t *cpu) {
    if (flagC()) {
        call_nn(cpu);
    } else {
        numCycles = 12;
        cpu->regs.pc += 3;
    }
}

// RESTARTS

static void rst(uint8_t val, gb_t *cpu) {
    push(++cpu->regs.pc, cpu);
    cpu->regs.pc = val;
}

static void rst_00(gb_t *cpu) {
    rst(0x00, cpu);
}

static void rst_10(gb_t *cpu) {
    rst(0x10, cpu);
}

static void rst_20(gb_t *cpu) {
    rst(0x20, cpu);
}

static void rst_30(gb_t *cpu) {
    rst(0x30, cpu);
}

static void rst_08(gb_t *cpu) {
    rst(0x08, cpu);
}

static void rst_18(gb_t *cpu) {
    rst(0x18, cpu);
}

static void rst_28(gb_t *cpu) {
    rst(0x28, cpu);
}

static void rst_38(gb_t *cpu) {
    rst(0x38, cpu);
}

// RETURNS

static void ret(gb_t *cpu) {
    cpu->regs.pc = pop(cpu);
    numCycles = 16;
}

static void ret_nz(gb_t *cpu) {
    if (!flagZ()) {
        ret(cpu);
        numCycles = 20;
    } else {
        cpu->regs.pc++;
        numCycles = 8;
    }
}

static void ret_z(gb_t *cpu) {
    if (flagZ()) {
        ret(cpu);
        numCycles = 20;
    } else {
        cpu->regs.pc++;
        numCycles = 8;
    }
}

static void ret_nc(gb_t *cpu) {
    if (!flagC()) {
        ret(cpu);
        numCycles = 20;
    } else {
        cpu->regs.pc++;
        numCycles = 8;
    }
}

static void ret_c(gb_t *cpu) {
    if (flagC()) {
        ret(cpu);
        numCycles = 20;
    } else {
        cpu->regs.pc++;
        numCycles = 8;
    }
}

static void reti(gb_t *cpu) {
    ret(cpu);
    cpu->interEnableBuffer = true;
}

// INTERUPTS

static void di(gb_t *cpu) {
    cpu->interrupts = false;
    cpu->interEnableBuffer = false;
    numCycles = 4;
    cpu->regs.pc++;
}

static void ei(gb_t *cpu) {
    cpu->interEnableBuffer = true;
    numCycles = 4;
    cpu->regs.pc++;
}

// MISCELLANEOUS

static void halt(gb_t *cpu) {
    cpu->halted = true;
    numCycles = 4;
    cpu->regs.pc++;
}

static void stop(gb_t *cpu) {
    cpu->stopped = true;
    numCycles = 4;
    cpu->regs.pc += 2;
}

// ROTATES AND SHIFTS

// Rotate A left, old 7 bit to carry flag
static void rlca(gb_t *cpu) {
    setC(!!(cpu->regs.a & 0x80));
    cpu->regs.a = (cpu->regs.a << 1) | flagC();
    setH(false);
    setN(false);
    setZ(false);
    numCycles = 4;
    cpu->regs.pc++;
}

// Rotate A left through carry flag
static void rla(gb_t *cpu) {
    bool tempC = flagC();
    setC(!!(cpu->regs.a & 0x80));
    cpu->regs.a = (cpu->regs.a << 1) | tempC;
    setH(false);
    setN(false);
    setZ(false);
    numCycles = 4;
    cpu->regs.pc++;
}

// Rotate A right, old 0 bit to carry flag
static void rrca(gb_t *cpu) {
    setC(!!(cpu->regs.a & 0x1));
    cpu->regs.a = (cpu->regs.a >> 1) | (flagC() << 7);
    setH(false);
    setN(false);
    setZ(false);
    numCycles = 4;
    cpu->regs.pc++;
}

// Rotate A right through carry flag
static void rra(gb_t *cpu) {
    bool tempC = flagC();
    setC(!!(cpu->regs.a & 0x1));
    cpu->regs.a = (cpu->regs.a >> 1) | (tempC << 7);
    setH(false);
    setN(false);
    setZ(false);
    numCycles = 4;
    cpu->regs.pc++;
}

// CB INSTRUCTIONS

// ROTATES AND SHIFTS

// Shift left by 1, old bit 7 goes into the carry flag and new bit 0
static void rlc (uint8_t *val, gb_t *cpu) {
    setC(!!(*val & 0x80));
    *val = (*val << 1) | flagC();
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void rlc_b (gb_t *cpu) {
    rlc(&cpu->regs.b, cpu);
}

static void rlc_c (gb_t *cpu) {
    rlc(&cpu->regs.c, cpu);
}

static void rlc_d (gb_t *cpu) {
    rlc(&cpu->regs.d, cpu);
}

static void rlc_e (gb_t *cpu) {
    rlc(&cpu->regs.e, cpu);
}

static void rlc_h (gb_t *cpu) {
    rlc(&cpu->regs.h, cpu);
}

static void rlc_l (gb_t *cpu) {
    rlc(&cpu->regs.l, cpu);
}

static void rlc_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    rlc(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void rlc_a (gb_t *cpu) {
    rlc(&cpu->regs.a, cpu);
}

// Shift 1 right, old bit 0 goes into carry flag and bit 7

static void rrc (uint8_t *val, gb_t *cpu) {
    setC(!!(*val & 0x1));
    *val = (*val >> 1) | (flagC() << 7);
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void rrc_b (gb_t *cpu) {
    rrc(&cpu->regs.b, cpu);
}

static void rrc_c (gb_t *cpu) {
    rrc(&cpu->regs.c, cpu);
}

static void rrc_d (gb_t *cpu) {
    rrc(&cpu->regs.d, cpu);
}

static void rrc_e (gb_t *cpu) {
    rrc(&cpu->regs.e, cpu);
}

static void rrc_h (gb_t *cpu) {
    rrc(&cpu->regs.h, cpu);
}

static void rrc_l (gb_t *cpu) {
    rrc(&cpu->regs.l, cpu);
}

static void rrc_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    rrc(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void rrc_a (gb_t *cpu) {
    rrc(&cpu->regs.a, cpu);
}

// Shift left by 1, through the carry flag, old bit 7 goes into the carry flag
// old carry flag becomes new bit 0

static void rl (uint8_t *val, gb_t *cpu) {
    bool tempC = flagC();
    setC(!!(*val & 0x80));
    *val = (*val << 1) | tempC;
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void rl_b (gb_t *cpu) {
    rl(&cpu->regs.b, cpu);
}

static void rl_c (gb_t *cpu) {
    rl(&cpu->regs.c, cpu);
}

static void rl_d (gb_t *cpu) {
    rl(&cpu->regs.d, cpu);
}

static void rl_e (gb_t *cpu) {
    rl(&cpu->regs.e, cpu);
}

static void rl_h (gb_t *cpu) {
    rl(&cpu->regs.h, cpu);
}

static void rl_l (gb_t *cpu) {
    rl(&cpu->regs.l, cpu);
}

static void rl_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    rl(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void rl_a (gb_t *cpu) {
    rl(&cpu->regs.a, cpu);
}

// Shift left by 1, through the carry flag, old bit 7 goes into the carry flag
// old carry flag becomes new bit 0

static void rr (uint8_t *val, gb_t *cpu) {
    bool tempC = flagC();
    setC(!!(*val & 0x1));
    *val = (*val >> 1) | (tempC << 7);
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void rr_b (gb_t *cpu) {
    rr(&cpu->regs.b, cpu);
}

static void rr_c (gb_t *cpu) {
    rr(&cpu->regs.c, cpu);
}

static void rr_d (gb_t *cpu) {
    rr(&cpu->regs.d, cpu);
}

static void rr_e (gb_t *cpu) {
    rr(&cpu->regs.e, cpu);
}

static void rr_h (gb_t *cpu) {
    rr(&cpu->regs.h, cpu);
}

static void rr_l (gb_t *cpu) {
    rr(&cpu->regs.l, cpu);
}

static void rr_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    rr(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void rr_a (gb_t *cpu) {
    rr(&cpu->regs.a, cpu);
}

// Shift left with 7 bit going into the carry flag, and the new 0 bit being set
// to 0

static void sla (uint8_t *val, gb_t *cpu) {
    setC(!!(*val & 0x80));
    *val <<= 1;
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void sla_b (gb_t *cpu) {
    sla(&cpu->regs.b, cpu);
}

static void sla_c (gb_t *cpu) {
    sla(&cpu->regs.c, cpu);
}

static void sla_d (gb_t *cpu) {
    sla(&cpu->regs.d, cpu);
}

static void sla_e (gb_t *cpu) {
    sla(&cpu->regs.e, cpu);
}

static void sla_h (gb_t *cpu) {
    sla(&cpu->regs.h, cpu);
}

static void sla_l (gb_t *cpu) {
    sla(&cpu->regs.l, cpu);
}

static void sla_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    sla(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void sla_a (gb_t *cpu) {
    sla(&cpu->regs.a, cpu);
}

// Shift right with 0 bit going into the carry flag, and the new 7 bit being
// unchanged

static void sra (uint8_t *val, gb_t *cpu) {
    setC(!!(*val & 0x1));
    *val = (*val >> 1) | (*val & 0x80);
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void sra_b (gb_t *cpu) {
    sra(&cpu->regs.b, cpu);
}

static void sra_c (gb_t *cpu) {
    sra(&cpu->regs.c, cpu);
}

static void sra_d (gb_t *cpu) {
    sra(&cpu->regs.d, cpu);
}

static void sra_e (gb_t *cpu) {
    sra(&cpu->regs.e, cpu);
}

static void sra_h (gb_t *cpu) {
    sra(&cpu->regs.h, cpu);
}

static void sra_l (gb_t *cpu) {
    sra(&cpu->regs.l, cpu);
}

static void sra_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    sra(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void sra_a (gb_t *cpu) {
    sra(&cpu->regs.a, cpu);
}

// Swap the lower and upper nibbles of a number

static void swap (uint8_t *val, gb_t *cpu) {
    *val = ((*val & 0xf0) >> 4) | ((*val & 0xf) << 4);
    setZ(*val == 0);
    setC(false);
    setN(false);
    setH(false);
    cpu->regs.pc++;
}

static void swap_b (gb_t *cpu) {
    swap(&cpu->regs.b, cpu);
}

static void swap_c (gb_t *cpu) {
    swap(&cpu->regs.c, cpu);
}

static void swap_d (gb_t *cpu) {
    swap(&cpu->regs.d, cpu);
}

static void swap_e (gb_t *cpu) {
    swap(&cpu->regs.e, cpu);
}

static void swap_h (gb_t *cpu) {
    swap(&cpu->regs.h, cpu);
}

static void swap_l (gb_t *cpu) {
    swap(&cpu->regs.l, cpu);
}

static void swap_hl (gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    swap(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void swap_a (gb_t *cpu) {
    swap(&cpu->regs.a, cpu);
}

// Shift right with 0 bit going into the carry flag, and the new 7 bit being set
// to 0

static void srl (uint8_t *val, gb_t *cpu) {
    setC(!!(*val & 0x01));
    *val >>= 1;
    setH(false);
    setN(false);
    setZ(*val == 0);
    cpu->regs.pc++;
}

static void srl_b(gb_t *cpu) {
    srl(&cpu->regs.b, cpu);
}

static void srl_c(gb_t *cpu) {
    srl(&cpu->regs.c, cpu);
}

static void srl_d(gb_t *cpu) {
    srl(&cpu->regs.d, cpu);
}

static void srl_e(gb_t *cpu) {
    srl(&cpu->regs.e, cpu);
}

static void srl_h(gb_t *cpu) {
    srl(&cpu->regs.h, cpu);
}

static void srl_l(gb_t *cpu) {
    srl(&cpu->regs.l, cpu);
}

static void srl_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    srl(&val, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void srl_a(gb_t *cpu) {
    srl(&cpu->regs.a, cpu);
}

// BITS OPCODES

// Check if the bit is on or not
static void bit(uint8_t val, uint8_t bit, gb_t *cpu) {
    setZ(!(val & (1 << bit)));
    setH(true);
    setN(false);
    cpu->regs.pc++;
}

static void bit_0_b(gb_t *cpu) {
    bit(cpu->regs.b, 0, cpu);
}

static void bit_0_c(gb_t *cpu) {
    bit(cpu->regs.c, 0, cpu);
}

static void bit_0_d(gb_t *cpu) {
    bit(cpu->regs.d, 0, cpu);
}

static void bit_0_e(gb_t *cpu) {
    bit(cpu->regs.e, 0, cpu);
}

static void bit_0_h(gb_t *cpu) {
    bit(cpu->regs.h, 0, cpu);
}

static void bit_0_l(gb_t *cpu) {
    bit(cpu->regs.l, 0, cpu);
}

static void bit_0_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 0, cpu);
}

static void bit_0_a(gb_t *cpu) {
    bit(cpu->regs.a, 0, cpu);
}

static void bit_1_b(gb_t *cpu) {
    bit(cpu->regs.b, 1, cpu);
}

static void bit_1_c(gb_t *cpu) {
    bit(cpu->regs.c, 1, cpu);
}

static void bit_1_d(gb_t *cpu) {
    bit(cpu->regs.d, 1, cpu);
}

static void bit_1_e(gb_t *cpu) {
    bit(cpu->regs.e, 1, cpu);
}

static void bit_1_h(gb_t *cpu) {
    bit(cpu->regs.h, 1, cpu);
}

static void bit_1_l(gb_t *cpu) {
    bit(cpu->regs.l, 1, cpu);
}

static void bit_1_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 1, cpu);
}

static void bit_1_a(gb_t *cpu) {
    bit(cpu->regs.a, 1, cpu);
}

static void bit_2_b(gb_t *cpu) {
    bit(cpu->regs.b, 2, cpu);
}

static void bit_2_c(gb_t *cpu) {
    bit(cpu->regs.c, 2, cpu);
}

static void bit_2_d(gb_t *cpu) {
    bit(cpu->regs.d, 2, cpu);
}

static void bit_2_e(gb_t *cpu) {
    bit(cpu->regs.e, 2, cpu);
}

static void bit_2_h(gb_t *cpu) {
    bit(cpu->regs.h, 2, cpu);
}

static void bit_2_l(gb_t *cpu) {
    bit(cpu->regs.l, 2, cpu);
}

static void bit_2_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 2, cpu);
}

static void bit_2_a(gb_t *cpu) {
    bit(cpu->regs.a, 2, cpu);
}

static void bit_3_b(gb_t *cpu) {
    bit(cpu->regs.b, 3, cpu);
}

static void bit_3_c(gb_t *cpu) {
    bit(cpu->regs.c, 3, cpu);
}

static void bit_3_d(gb_t *cpu) {
    bit(cpu->regs.d, 3, cpu);
}

static void bit_3_e(gb_t *cpu) {
    bit(cpu->regs.e, 3, cpu);
}

static void bit_3_h(gb_t *cpu) {
    bit(cpu->regs.h, 3, cpu);
}

static void bit_3_l(gb_t *cpu) {
    bit(cpu->regs.l, 3, cpu);
}

static void bit_3_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 3, cpu);
}

static void bit_3_a(gb_t *cpu) {
    bit(cpu->regs.a, 3, cpu);
}

static void bit_4_b(gb_t *cpu) {
    bit(cpu->regs.b, 4, cpu);
}

static void bit_4_c(gb_t *cpu) {
    bit(cpu->regs.c, 4, cpu);
}

static void bit_4_d(gb_t *cpu) {
    bit(cpu->regs.d, 4, cpu);
}

static void bit_4_e(gb_t *cpu) {
    bit(cpu->regs.e, 4, cpu);
}

static void bit_4_h(gb_t *cpu) {
    bit(cpu->regs.h, 4, cpu);
}

static void bit_4_l(gb_t *cpu) {
    bit(cpu->regs.l, 4, cpu);
}

static void bit_4_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 4, cpu);
}

static void bit_4_a(gb_t *cpu) {
    bit(cpu->regs.a, 4, cpu);
}

static void bit_5_b(gb_t *cpu) {
    bit(cpu->regs.b, 5, cpu);
}

static void bit_5_c(gb_t *cpu) {
    bit(cpu->regs.c, 5, cpu);
}

static void bit_5_d(gb_t *cpu) {
    bit(cpu->regs.d, 5, cpu);
}

static void bit_5_e(gb_t *cpu) {
    bit(cpu->regs.e, 5, cpu);
}

static void bit_5_h(gb_t *cpu) {
    bit(cpu->regs.h, 5, cpu);
}

static void bit_5_l(gb_t *cpu) {
    bit(cpu->regs.l, 5, cpu);
}

static void bit_5_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 5, cpu);
}

static void bit_5_a(gb_t *cpu) {
    bit(cpu->regs.a, 5, cpu);
}

static void bit_6_b(gb_t *cpu) {
    bit(cpu->regs.b, 6, cpu);
}

static void bit_6_c(gb_t *cpu) {
    bit(cpu->regs.c, 6, cpu);
}

static void bit_6_d(gb_t *cpu) {
    bit(cpu->regs.d, 6, cpu);
}

static void bit_6_e(gb_t *cpu) {
    bit(cpu->regs.e, 6, cpu);
}

static void bit_6_h(gb_t *cpu) {
    bit(cpu->regs.h, 6, cpu);
}

static void bit_6_l(gb_t *cpu) {
    bit(cpu->regs.l, 6, cpu);
}

static void bit_6_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 6, cpu);
}

static void bit_6_a(gb_t *cpu) {
    bit(cpu->regs.a, 6, cpu);
}

static void bit_7_b(gb_t *cpu) {
    bit(cpu->regs.b, 7, cpu);
}

static void bit_7_c(gb_t *cpu) {
    bit(cpu->regs.c, 7, cpu);
}

static void bit_7_d(gb_t *cpu) {
    bit(cpu->regs.d, 7, cpu);
}

static void bit_7_e(gb_t *cpu) {
    bit(cpu->regs.e, 7, cpu);
}

static void bit_7_h(gb_t *cpu) {
    bit(cpu->regs.h, 7, cpu);
}

static void bit_7_l(gb_t *cpu) {
    bit(cpu->regs.l, 7, cpu);
}

static void bit_7_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    numCycles += 4;
    bit(val, 7, cpu);
}

static void bit_7_a(gb_t *cpu) {
    bit(cpu->regs.a, 7, cpu);
}

// Sets the bit to 0
static void res(uint8_t *val, uint8_t bit, gb_t *cpu) {
    *val &= ~(1 << bit);
    cpu->regs.pc++;
}

static void res_0_b(gb_t *cpu) {
    res(&cpu->regs.b, 0, cpu);
}

static void res_0_c(gb_t *cpu) {
    res(&cpu->regs.c, 0, cpu);
}

static void res_0_d(gb_t *cpu) {
    res(&cpu->regs.d, 0, cpu);
}

static void res_0_e(gb_t *cpu) {
    res(&cpu->regs.e, 0, cpu);
}

static void res_0_h(gb_t *cpu) {
    res(&cpu->regs.h, 0, cpu);
}

static void res_0_l(gb_t *cpu) {
    res(&cpu->regs.l, 0, cpu);
}

static void res_0_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 0, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_0_a(gb_t *cpu) {
    res(&cpu->regs.a, 0, cpu);
}

static void res_1_b(gb_t *cpu) {
    res(&cpu->regs.b, 1, cpu);
}

static void res_1_c(gb_t *cpu) {
    res(&cpu->regs.c, 1, cpu);
}

static void res_1_d(gb_t *cpu) {
    res(&cpu->regs.d, 1, cpu);
}

static void res_1_e(gb_t *cpu) {
    res(&cpu->regs.e, 1, cpu);
}

static void res_1_h(gb_t *cpu) {
    res(&cpu->regs.h, 1, cpu);
}

static void res_1_l(gb_t *cpu) {
    res(&cpu->regs.l, 1, cpu);
}

static void res_1_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 1, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_1_a(gb_t *cpu) {
    res(&cpu->regs.a, 1, cpu);
}

static void res_2_b(gb_t *cpu) {
    res(&cpu->regs.b, 2, cpu);
}

static void res_2_c(gb_t *cpu) {
    res(&cpu->regs.c, 2, cpu);
}

static void res_2_d(gb_t *cpu) {
    res(&cpu->regs.d, 2, cpu);
}

static void res_2_e(gb_t *cpu) {
    res(&cpu->regs.e, 2, cpu);
}

static void res_2_h(gb_t *cpu) {
    res(&cpu->regs.h, 2, cpu);
}

static void res_2_l(gb_t *cpu) {
    res(&cpu->regs.l, 2, cpu);
}

static void res_2_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 2, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_2_a(gb_t *cpu) {
    res(&cpu->regs.a, 2, cpu);
}

static void res_3_b(gb_t *cpu) {
    res(&cpu->regs.b, 3, cpu);
}

static void res_3_c(gb_t *cpu) {
    res(&cpu->regs.c, 3, cpu);
}

static void res_3_d(gb_t *cpu) {
    res(&cpu->regs.d, 3, cpu);
}

static void res_3_e(gb_t *cpu) {
    res(&cpu->regs.e, 3, cpu);
}

static void res_3_h(gb_t *cpu) {
    res(&cpu->regs.h, 3, cpu);
}

static void res_3_l(gb_t *cpu) {
    res(&cpu->regs.l, 3, cpu);
}

static void res_3_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 3, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_3_a(gb_t *cpu) {
    res(&cpu->regs.a, 3, cpu);
}

static void res_4_b(gb_t *cpu) {
    res(&cpu->regs.b, 4, cpu);
}

static void res_4_c(gb_t *cpu) {
    res(&cpu->regs.c, 4, cpu);
}

static void res_4_d(gb_t *cpu) {
    res(&cpu->regs.d, 4, cpu);
}

static void res_4_e(gb_t *cpu) {
    res(&cpu->regs.e, 4, cpu);
}

static void res_4_h(gb_t *cpu) {
    res(&cpu->regs.h, 4, cpu);
}

static void res_4_l(gb_t *cpu) {
    res(&cpu->regs.l, 4, cpu);
}

static void res_4_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 4, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_4_a(gb_t *cpu) {
    res(&cpu->regs.a, 4, cpu);
}

static void res_5_b(gb_t *cpu) {
    res(&cpu->regs.b, 5, cpu);
}

static void res_5_c(gb_t *cpu) {
    res(&cpu->regs.c, 5, cpu);
}

static void res_5_d(gb_t *cpu) {
    res(&cpu->regs.d, 5, cpu);
}

static void res_5_e(gb_t *cpu) {
    res(&cpu->regs.e, 5, cpu);
}

static void res_5_h(gb_t *cpu) {
    res(&cpu->regs.h, 5, cpu);
}

static void res_5_l(gb_t *cpu) {
    res(&cpu->regs.l, 5, cpu);
}

static void res_5_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 5, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_5_a(gb_t *cpu) {
    res(&cpu->regs.a, 5, cpu);
}

static void res_6_b(gb_t *cpu) {
    res(&cpu->regs.b, 6, cpu);
}

static void res_6_c(gb_t *cpu) {
    res(&cpu->regs.c, 6, cpu);
}

static void res_6_d(gb_t *cpu) {
    res(&cpu->regs.d, 6, cpu);
}

static void res_6_e(gb_t *cpu) {
    res(&cpu->regs.e, 6, cpu);
}

static void res_6_h(gb_t *cpu) {
    res(&cpu->regs.h, 6, cpu);
}

static void res_6_l(gb_t *cpu) {
    res(&cpu->regs.l, 6, cpu);
}

static void res_6_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 6, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_6_a(gb_t *cpu) {
    res(&cpu->regs.a, 6, cpu);
}

static void res_7_b(gb_t *cpu) {
    res(&cpu->regs.b, 7, cpu);
}

static void res_7_c(gb_t *cpu) {
    res(&cpu->regs.c, 7, cpu);
}

static void res_7_d(gb_t *cpu) {
    res(&cpu->regs.d, 7, cpu);
}

static void res_7_e(gb_t *cpu) {
    res(&cpu->regs.e, 7, cpu);
}

static void res_7_h(gb_t *cpu) {
    res(&cpu->regs.h, 7, cpu);
}

static void res_7_l(gb_t *cpu) {
    res(&cpu->regs.l, 7, cpu);
}

static void res_7_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    res(&val, 7, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void res_7_a(gb_t *cpu) {
    res(&cpu->regs.a, 7, cpu);
}

// Sets the bit to 1
static void set(uint8_t *val, uint8_t bit, gb_t *cpu) {
    *val |= (1 << bit);
    cpu->regs.pc++;
}

static void set_0_b(gb_t *cpu) {
    set(&cpu->regs.b, 0, cpu);
}

static void set_0_c(gb_t *cpu) {
    set(&cpu->regs.c, 0, cpu);
}

static void set_0_d(gb_t *cpu) {
    set(&cpu->regs.d, 0, cpu);
}

static void set_0_e(gb_t *cpu) {
    set(&cpu->regs.e, 0, cpu);
}

static void set_0_h(gb_t *cpu) {
    set(&cpu->regs.h, 0, cpu);
}

static void set_0_l(gb_t *cpu) {
    set(&cpu->regs.l, 0, cpu);
}

static void set_0_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 0, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_0_a(gb_t *cpu) {
    set(&cpu->regs.a, 0, cpu);
}

static void set_1_b(gb_t *cpu) {
    set(&cpu->regs.b, 1, cpu);
}

static void set_1_c(gb_t *cpu) {
    set(&cpu->regs.c, 1, cpu);
}

static void set_1_d(gb_t *cpu) {
    set(&cpu->regs.d, 1, cpu);
}

static void set_1_e(gb_t *cpu) {
    set(&cpu->regs.e, 1, cpu);
}

static void set_1_h(gb_t *cpu) {
    set(&cpu->regs.h, 1, cpu);
}

static void set_1_l(gb_t *cpu) {
    set(&cpu->regs.l, 1, cpu);
}

static void set_1_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 1, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_1_a(gb_t *cpu) {
    set(&cpu->regs.a, 1, cpu);
}

static void set_2_b(gb_t *cpu) {
    set(&cpu->regs.b, 2, cpu);
}

static void set_2_c(gb_t *cpu) {
    set(&cpu->regs.c, 2, cpu);
}

static void set_2_d(gb_t *cpu) {
    set(&cpu->regs.d, 2, cpu);
}

static void set_2_e(gb_t *cpu) {
    set(&cpu->regs.e, 2, cpu);
}

static void set_2_h(gb_t *cpu) {
    set(&cpu->regs.h, 2, cpu);
}

static void set_2_l(gb_t *cpu) {
    set(&cpu->regs.l, 2, cpu);
}

static void set_2_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 2, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_2_a(gb_t *cpu) {
    set(&cpu->regs.a, 2, cpu);
}

static void set_3_b(gb_t *cpu) {
    set(&cpu->regs.b, 3, cpu);
}

static void set_3_c(gb_t *cpu) {
    set(&cpu->regs.c, 3, cpu);
}

static void set_3_d(gb_t *cpu) {
    set(&cpu->regs.d, 3, cpu);
}

static void set_3_e(gb_t *cpu) {
    set(&cpu->regs.e, 3, cpu);
}

static void set_3_h(gb_t *cpu) {
    set(&cpu->regs.h, 3, cpu);
}

static void set_3_l(gb_t *cpu) {
    set(&cpu->regs.l, 3, cpu);
}

static void set_3_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 3, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_3_a(gb_t *cpu) {
    set(&cpu->regs.a, 3, cpu);
}

static void set_4_b(gb_t *cpu) {
    set(&cpu->regs.b, 4, cpu);
}

static void set_4_c(gb_t *cpu) {
    set(&cpu->regs.c, 4, cpu);
}

static void set_4_d(gb_t *cpu) {
    set(&cpu->regs.d, 4, cpu);
}

static void set_4_e(gb_t *cpu) {
    set(&cpu->regs.e, 4, cpu);
}

static void set_4_h(gb_t *cpu) {
    set(&cpu->regs.h, 4, cpu);
}

static void set_4_l(gb_t *cpu) {
    set(&cpu->regs.l, 4, cpu);
}

static void set_4_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 4, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_4_a(gb_t *cpu) {
    set(&cpu->regs.a, 4, cpu);
}

static void set_5_b(gb_t *cpu) {
    set(&cpu->regs.b, 5, cpu);
}

static void set_5_c(gb_t *cpu) {
    set(&cpu->regs.c, 5, cpu);
}

static void set_5_d(gb_t *cpu) {
    set(&cpu->regs.d, 5, cpu);
}

static void set_5_e(gb_t *cpu) {
    set(&cpu->regs.e, 5, cpu);
}

static void set_5_h(gb_t *cpu) {
    set(&cpu->regs.h, 5, cpu);
}

static void set_5_l(gb_t *cpu) {
    set(&cpu->regs.l, 5, cpu);
}

static void set_5_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 5, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_5_a(gb_t *cpu) {
    set(&cpu->regs.a, 5, cpu);
}

static void set_6_b(gb_t *cpu) {
    set(&cpu->regs.b, 6, cpu);
}

static void set_6_c(gb_t *cpu) {
    set(&cpu->regs.c, 6, cpu);
}

static void set_6_d(gb_t *cpu) {
    set(&cpu->regs.d, 6, cpu);
}

static void set_6_e(gb_t *cpu) {
    set(&cpu->regs.e, 6, cpu);
}

static void set_6_h(gb_t *cpu) {
    set(&cpu->regs.h, 6, cpu);
}

static void set_6_l(gb_t *cpu) {
    set(&cpu->regs.l, 6, cpu);
}

static void set_6_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 6, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_6_a(gb_t *cpu) {
    set(&cpu->regs.a, 6, cpu);
}

static void set_7_b(gb_t *cpu) {
    set(&cpu->regs.b, 7, cpu);
}

static void set_7_c(gb_t *cpu) {
    set(&cpu->regs.c, 7, cpu);
}

static void set_7_d(gb_t *cpu) {
    set(&cpu->regs.d, 7, cpu);
}

static void set_7_e(gb_t *cpu) {
    set(&cpu->regs.e, 7, cpu);
}

static void set_7_h(gb_t *cpu) {
    set(&cpu->regs.h, 7, cpu);
}

static void set_7_l(gb_t *cpu) {
    set(&cpu->regs.l, 7, cpu);
}

static void set_7_hl(gb_t *cpu) {
    incAllTimers(4);
    uint8_t val = busRead8(regHL());
    set(&val, 7, cpu);
    incAllTimers(4);
    busWrite8(regHL(), val);
}

static void set_7_a(gb_t *cpu) {
    set(&cpu->regs.a, 7, cpu);
}

static inst cb_instr[0x100] = {
    //0x00 - 0x0f
    [0x00] = &rlc_b,
    [0x01] = &rlc_c,
    [0x02] = &rlc_d,
    [0x03] = &rlc_e,
    [0x04] = &rlc_h,
    [0x05] = &rlc_l,
    [0x06] = &rlc_hl,
    [0x07] = &rlc_a,
    [0x08] = &rrc_b,
    [0x09] = &rrc_c,
    [0x0a] = &rrc_d,
    [0x0b] = &rrc_e,
    [0x0c] = &rrc_h,
    [0x0d] = &rrc_l,
    [0x0e] = &rrc_hl,
    [0x0f] = &rrc_a,

    //0x10 - 0x1f
    [0x10] = &rl_b,
    [0x11] = &rl_c,
    [0x12] = &rl_d,
    [0x13] = &rl_e,
    [0x14] = &rl_h,
    [0x15] = &rl_l,
    [0x16] = &rl_hl,
    [0x17] = &rl_a,
    [0x18] = &rr_b,
    [0x19] = &rr_c,
    [0x1a] = &rr_d,
    [0x1b] = &rr_e,
    [0x1c] = &rr_h,
    [0x1d] = &rr_l,
    [0x1e] = &rr_hl,
    [0x1f] = &rr_a,

    //0x20 - 0x2f
    [0x20] = &sla_b,
    [0x21] = &sla_c,
    [0x22] = &sla_d,
    [0x23] = &sla_e,
    [0x24] = &sla_h,
    [0x25] = &sla_l,
    [0x26] = &sla_hl,
    [0x27] = &sla_a,
    [0x28] = &sra_b,
    [0x29] = &sra_c,
    [0x2a] = &sra_d,
    [0x2b] = &sra_e,
    [0x2c] = &sra_h,
    [0x2d] = &sra_l,
    [0x2e] = &sra_hl,
    [0x2f] = &sra_a,

    //0x30 - 0x3f
    [0x30] = &swap_b,
    [0x31] = &swap_c,
    [0x32] = &swap_d,
    [0x33] = &swap_e,
    [0x34] = &swap_h,
    [0x35] = &swap_l,
    [0x36] = &swap_hl,
    [0x37] = &swap_a,
    [0x38] = &srl_b,
    [0x39] = &srl_c,
    [0x3a] = &srl_d,
    [0x3b] = &srl_e,
    [0x3c] = &srl_h,
    [0x3d] = &srl_l,
    [0x3e] = &srl_hl,
    [0x3f] = &srl_a,

    //0x40 - 0x4f
    [0x40] = &bit_0_b,
    [0x41] = &bit_0_c,
    [0x42] = &bit_0_d,
    [0x43] = &bit_0_e,
    [0x44] = &bit_0_h,
    [0x45] = &bit_0_l,
    [0x46] = &bit_0_hl,
    [0x47] = &bit_0_a,
    [0x48] = &bit_1_b,
    [0x49] = &bit_1_c,
    [0x4a] = &bit_1_d,
    [0x4b] = &bit_1_e,
    [0x4c] = &bit_1_h,
    [0x4d] = &bit_1_l,
    [0x4e] = &bit_1_hl,
    [0x4f] = &bit_1_a,

    //0x50 - 0x5f
    [0x50] = &bit_2_b,
    [0x51] = &bit_2_c,
    [0x52] = &bit_2_d,
    [0x53] = &bit_2_e,
    [0x54] = &bit_2_h,
    [0x55] = &bit_2_l,
    [0x56] = &bit_2_hl,
    [0x57] = &bit_2_a,
    [0x58] = &bit_3_b,
    [0x59] = &bit_3_c,
    [0x5a] = &bit_3_d,
    [0x5b] = &bit_3_e,
    [0x5c] = &bit_3_h,
    [0x5d] = &bit_3_l,
    [0x5e] = &bit_3_hl,
    [0x5f] = &bit_3_a,

    //0x60 - 0x6f
    [0x60] = &bit_4_b,
    [0x61] = &bit_4_c,
    [0x62] = &bit_4_d,
    [0x63] = &bit_4_e,
    [0x64] = &bit_4_h,
    [0x65] = &bit_4_l,
    [0x66] = &bit_4_hl,
    [0x67] = &bit_4_a,
    [0x68] = &bit_5_b,
    [0x69] = &bit_5_c,
    [0x6a] = &bit_5_d,
    [0x6b] = &bit_5_e,
    [0x6c] = &bit_5_h,
    [0x6d] = &bit_5_l,
    [0x6e] = &bit_5_hl,
    [0x6f] = &bit_5_a,

    //0x70 - 0x7f
    [0x70] = &bit_6_b,
    [0x71] = &bit_6_c,
    [0x72] = &bit_6_d,
    [0x73] = &bit_6_e,
    [0x74] = &bit_6_h,
    [0x75] = &bit_6_l,
    [0x76] = &bit_6_hl,
    [0x77] = &bit_6_a,
    [0x78] = &bit_7_b,
    [0x79] = &bit_7_c,
    [0x7a] = &bit_7_d,
    [0x7b] = &bit_7_e,
    [0x7c] = &bit_7_h,
    [0x7d] = &bit_7_l,
    [0x7e] = &bit_7_hl,
    [0x7f] = &bit_7_a,

    //0x80 - 0x8f
    [0x80] = &res_0_b,
    [0x81] = &res_0_c,
    [0x82] = &res_0_d,
    [0x83] = &res_0_e,
    [0x84] = &res_0_h,
    [0x85] = &res_0_l,
    [0x86] = &res_0_hl,
    [0x87] = &res_0_a,
    [0x88] = &res_1_b,
    [0x89] = &res_1_c,
    [0x8a] = &res_1_d,
    [0x8b] = &res_1_e,
    [0x8c] = &res_1_h,
    [0x8d] = &res_1_l,
    [0x8e] = &res_1_hl,
    [0x8f] = &res_1_a,

    //0x90 - 0x9f
    [0x90] = &res_2_b,
    [0x91] = &res_2_c,
    [0x92] = &res_2_d,
    [0x93] = &res_2_e,
    [0x94] = &res_2_h,
    [0x95] = &res_2_l,
    [0x96] = &res_2_hl,
    [0x97] = &res_2_a,
    [0x98] = &res_3_b,
    [0x99] = &res_3_c,
    [0x9a] = &res_3_d,
    [0x9b] = &res_3_e,
    [0x9c] = &res_3_h,
    [0x9d] = &res_3_l,
    [0x9e] = &res_3_hl,
    [0x9f] = &res_3_a,

    //0xa0 - 0xaf
    [0xa0] = &res_4_b,
    [0xa1] = &res_4_c,
    [0xa2] = &res_4_d,
    [0xa3] = &res_4_e,
    [0xa4] = &res_4_h,
    [0xa5] = &res_4_l,
    [0xa6] = &res_4_hl,
    [0xa7] = &res_4_a,
    [0xa8] = &res_5_b,
    [0xa9] = &res_5_c,
    [0xaa] = &res_5_d,
    [0xab] = &res_5_e,
    [0xac] = &res_5_h,
    [0xad] = &res_5_l,
    [0xae] = &res_5_hl,
    [0xaf] = &res_5_a,

    //0xb0 - 0xbf
    [0xb0] = &res_6_b,
    [0xb1] = &res_6_c,
    [0xb2] = &res_6_d,
    [0xb3] = &res_6_e,
    [0xb4] = &res_6_h,
    [0xb5] = &res_6_l,
    [0xb6] = &res_6_hl,
    [0xb7] = &res_6_a,
    [0xb8] = &res_7_b,
    [0xb9] = &res_7_c,
    [0xba] = &res_7_d,
    [0xbb] = &res_7_e,
    [0xbc] = &res_7_h,
    [0xbd] = &res_7_l,
    [0xbe] = &res_7_hl,
    [0xbf] = &res_7_a,

    //0xc0 - 0xcf
    [0xc0] = &set_0_b,
    [0xc1] = &set_0_c,
    [0xc2] = &set_0_d,
    [0xc3] = &set_0_e,
    [0xc4] = &set_0_h,
    [0xc5] = &set_0_l,
    [0xc6] = &set_0_hl,
    [0xc7] = &set_0_a,
    [0xc8] = &set_1_b,
    [0xc9] = &set_1_c,
    [0xca] = &set_1_d,
    [0xcb] = &set_1_e,
    [0xcc] = &set_1_h,
    [0xcd] = &set_1_l,
    [0xce] = &set_1_hl,
    [0xcf] = &set_1_a,

    //0xd0 - 0xdf
    [0xd0] = &set_2_b,
    [0xd1] = &set_2_c,
    [0xd2] = &set_2_d,
    [0xd3] = &set_2_e,
    [0xd4] = &set_2_h,
    [0xd5] = &set_2_l,
    [0xd6] = &set_2_hl,
    [0xd7] = &set_2_a,
    [0xd8] = &set_3_b,
    [0xd9] = &set_3_c,
    [0xda] = &set_3_d,
    [0xdb] = &set_3_e,
    [0xdc] = &set_3_h,
    [0xdd] = &set_3_l,
    [0xde] = &set_3_hl,
    [0xdf] = &set_3_a,

    //0xe0 - 0xef
    [0xe0] = &set_4_b,
    [0xe1] = &set_4_c,
    [0xe2] = &set_4_d,
    [0xe3] = &set_4_e,
    [0xe4] = &set_4_h,
    [0xe5] = &set_4_l,
    [0xe6] = &set_4_hl,
    [0xe7] = &set_4_a,
    [0xe8] = &set_5_b,
    [0xe9] = &set_5_c,
    [0xea] = &set_5_d,
    [0xeb] = &set_5_e,
    [0xec] = &set_5_h,
    [0xed] = &set_5_l,
    [0xee] = &set_5_hl,
    [0xef] = &set_5_a,

    //0xf0 - 0xff
    [0xf0] = &set_6_b,
    [0xf1] = &set_6_c,
    [0xf2] = &set_6_d,
    [0xf3] = &set_6_e,
    [0xf4] = &set_6_h,
    [0xf5] = &set_6_l,
    [0xf6] = &set_6_hl,
    [0xf7] = &set_6_a,
    [0xf8] = &set_7_b,
    [0xf9] = &set_7_c,
    [0xfa] = &set_7_d,
    [0xfb] = &set_7_e,
    [0xfc] = &set_7_h,
    [0xfd] = &set_7_l,
    [0xfe] = &set_7_hl,
    [0xff] = &set_7_a,
};

static void cb(gb_t *cpu) {
    uint8_t op = busRead8(++cpu->regs.pc);
    numCycles = 8;
    cb_instr[op](cpu);
}

static inst instructions[0x100] = {
    // 0x00 - 0x0f
    [0x00] = &nop,
    [0x01] = &ld_bc_nn,
    [0x02] = &ld_bc_a,
    [0x03] = &inc_bc,
    [0x04] = &inc_b,
    [0x05] = &dec_b,
    [0x06] = &ld_b_n,
    [0x07] = &rlca,
    [0x08] = &ld_nn_sp,
    [0x09] = &add_hl_bc,
    [0x0a] = &ld_a_bc,
    [0x0b] = &dec_bc,
    [0x0c] = &inc_c,
    [0x0d] = &dec_c,
    [0x0e] = &ld_c_n,
    [0x0f] = &rrca,

    // 0x10 - 0x1f
    [0x10] = &stop,
    [0x11] = &ld_de_nn,
    [0x12] = &ld_de_a,
    [0x13] = &inc_de,
    [0x14] = &inc_d,
    [0x15] = &dec_d,
    [0x16] = &ld_d_n,
    [0x17] = &rla,
    [0x18] = &jr_n,
    [0x19] = &add_hl_de,
    [0x1a] = &ld_a_de,
    [0x1b] = &dec_de,
    [0x1c] = &inc_e,
    [0x1d] = &dec_e,
    [0x1e] = &ld_e_n,
    [0x1f] = &rra,

    // 0x20 - 0x2f
    [0x20] = &jr_nz_n,
    [0x21] = &ld_hl_nn,
    [0x22] = &ld_hli_a,
    [0x23] = &inc_hl,
    [0x24] = &inc_h,
    [0x25] = &dec_h,
    [0x26] = &ld_h_n,
    [0x27] = &daa,
    [0x28] = &jr_z_n,
    [0x29] = &add_hl_hl,
    [0x2a] = &ld_a_hli,
    [0x2b] = &dec_hl,
    [0x2c] = &inc_l,
    [0x2d] = &dec_l,
    [0x2e] = &ld_l_n,
    [0x2f] = &cpl,

    // 0x30 - 0x3f
    [0x30] = &jr_nc_n,
    [0x31] = &ld_sp_nn,
    [0x32] = &ld_hld_a,
    [0x33] = &inc_sp,
    [0x34] = &inc_addr_hl,
    [0x35] = &dec_addr_hl,
    [0x36] = &ld_hl_n,
    [0x37] = &scf,
    [0x38] = &jr_c_n,
    [0x39] = &add_hl_sp,
    [0x3a] = &ld_a_hld,
    [0x3b] = &dec_sp,
    [0x3c] = &inc_a,
    [0x3d] = &dec_a,
    [0x3e] = &ld_a_n,
    [0x3f] = &ccf,

    // 0x40 - 0x4f
    [0x40] = &ld_b_b,
    [0x41] = &ld_b_c,
    [0x42] = &ld_b_d,
    [0x43] = &ld_b_e,
    [0x44] = &ld_b_h,
    [0x45] = &ld_b_l,
    [0x46] = &ld_b_hl,
    [0x47] = &ld_b_a,
    [0x48] = &ld_c_b,
    [0x49] = &ld_c_c,
    [0x4a] = &ld_c_d,
    [0x4b] = &ld_c_e,
    [0x4c] = &ld_c_h,
    [0x4d] = &ld_c_l,
    [0x4e] = &ld_c_hl,
    [0x4f] = &ld_c_a,

    // 0x50 - 0x5f
    [0x50] = &ld_d_b,
    [0x51] = &ld_d_c,
    [0x52] = &ld_d_d,
    [0x53] = &ld_d_e,
    [0x54] = &ld_d_h,
    [0x55] = &ld_d_l,
    [0x56] = &ld_d_hl,
    [0x57] = &ld_d_a,
    [0x58] = &ld_e_b,
    [0x59] = &ld_e_c,
    [0x5a] = &ld_e_d,
    [0x5b] = &ld_e_e,
    [0x5c] = &ld_e_h,
    [0x5d] = &ld_e_l,
    [0x5e] = &ld_e_hl,
    [0x5f] = &ld_e_a,

    // 0x60 - 0x6f
    [0x60] = &ld_h_b,
    [0x61] = &ld_h_c,
    [0x62] = &ld_h_d,
    [0x63] = &ld_h_e,
    [0x64] = &ld_h_h,
    [0x65] = &ld_h_l,
    [0x66] = &ld_h_hl,
    [0x67] = &ld_h_a,
    [0x68] = &ld_l_b,
    [0x69] = &ld_l_c,
    [0x6a] = &ld_l_d,
    [0x6b] = &ld_l_e,
    [0x6c] = &ld_l_h,
    [0x6d] = &ld_l_l,
    [0x6e] = &ld_l_hl,
    [0x6f] = &ld_l_a,

    // 0x70 - 0x7f
    [0x70] = &ld_hl_b,
    [0x71] = &ld_hl_c,
    [0x72] = &ld_hl_d,
    [0x73] = &ld_hl_e,
    [0x74] = &ld_hl_h,
    [0x75] = &ld_hl_l,
    [0x76] = &halt,
    [0x77] = &ld_hl_a,
    [0x78] = &ld_a_b,
    [0x79] = &ld_a_c,
    [0x7a] = &ld_a_d,
    [0x7b] = &ld_a_e,
    [0x7c] = &ld_a_h,
    [0x7d] = &ld_a_l,
    [0x7e] = &ld_a_hl,
    [0x7f] = &ld_a_a,

    // 0x80 - 0x8f
    [0x80] = &add_a_b,
    [0x81] = &add_a_c,
    [0x82] = &add_a_d,
    [0x83] = &add_a_e,
    [0x84] = &add_a_h,
    [0x85] = &add_a_l,
    [0x86] = &add_a_hl,
    [0x87] = &add_a_a,
    [0x88] = &adc_a_b,
    [0x89] = &adc_a_c,
    [0x8a] = &adc_a_d,
    [0x8b] = &adc_a_e,
    [0x8c] = &adc_a_h,
    [0x8d] = &adc_a_l,
    [0x8e] = &adc_a_hl,
    [0x8f] = &adc_a_a,

    // 0x90 - 0x9f
    [0x90] = &sub_b,
    [0x91] = &sub_c,
    [0x92] = &sub_d,
    [0x93] = &sub_e,
    [0x94] = &sub_h,
    [0x95] = &sub_l,
    [0x96] = &sub_hl,
    [0x97] = &sub_a,
    [0x98] = &sbc_a_b,
    [0x99] = &sbc_a_c,
    [0x9a] = &sbc_a_d,
    [0x9b] = &sbc_a_e,
    [0x9c] = &sbc_a_h,
    [0x9d] = &sbc_a_l,
    [0x9e] = &sbc_a_hl,
    [0x9f] = &sbc_a_a,

    // 0xa0 - 0xaf
    [0xa0] = &and_b,
    [0xa1] = &and_c,
    [0xa2] = &and_d,
    [0xa3] = &and_e,
    [0xa4] = &and_h,
    [0xa5] = &and_l,
    [0xa6] = &and_hl,
    [0xa7] = &and_a,
    [0xa8] = &xor_b,
    [0xa9] = &xor_c,
    [0xaa] = &xor_d,
    [0xab] = &xor_e,
    [0xac] = &xor_h,
    [0xad] = &xor_l,
    [0xae] = &xor_hl,
    [0xaf] = &xor_a,

    // 0xb0 - 0xbf
    [0xb0] = &or_b,
    [0xb1] = &or_c,
    [0xb2] = &or_d,
    [0xb3] = &or_e,
    [0xb4] = &or_h,
    [0xb5] = &or_l,
    [0xb6] = &or_hl,
    [0xb7] = &or_a,
    [0xb8] = &cp_b,
    [0xb9] = &cp_c,
    [0xba] = &cp_d,
    [0xbb] = &cp_e,
    [0xbc] = &cp_h,
    [0xbd] = &cp_l,
    [0xbe] = &cp_hl,
    [0xbf] = &cp_a,

    // 0xc0 - 0xcf
    [0xc0] = &ret_nz,
    [0xc1] = &pop_bc,
    [0xc2] = &jp_nz_nn,
    [0xc3] = &jp_nn,
    [0xc4] = &call_nz_nn,
    [0xc5] = &push_bc,
    [0xc6] = &add_a_n,
    [0xc7] = &rst_00,
    [0xc8] = &ret_z,
    [0xc9] = &ret,
    [0xca] = &jp_z_nn,
    [0xcb] = &cb,
    [0xcc] = &call_z_nn,
    [0xcd] = &call_nn,
    [0xce] = &adc_a_n,
    [0xcf] = &rst_08,

    // 0xd0 - 0xdf
    [0xd0] = &ret_nc,
    [0xd1] = &pop_de,
    [0xd2] = &jp_nc_nn,
    [0xd4] = &call_nc_nn,
    [0xd5] = &push_de,
    [0xd6] = &sub_n,
    [0xd7] = &rst_10,
    [0xd8] = &ret_c,
    [0xd9] = &reti,
    [0xda] = &jp_c_nn,
    [0xdc] = &call_c_nn,
    [0xde] = &sbc_a_n,
    [0xdf] = &rst_18,

    // 0xe0 - 0xef
    [0xe0] = &ldh_n_a,
    [0xe1] = &pop_hl,
    [0xe2] = &ld_addr_c_a,
    [0xe5] = &push_hl,
    [0xe6] = &and_n,
    [0xe7] = &rst_20,
    [0xe8] = &add_sp_n,
    [0xe9] = &jp_hl,
    [0xea] = &ld_nn_a,
    [0xee] = &xor_n,
    [0xef] = &rst_28,

    // 0xf0 - 0xff
    [0xf0] = &ldh_a_n,
    [0xf1] = &pop_af,
    [0xf2] = &ld_a_addr_c,
    [0xf3] = &di,
    [0xf5] = &push_af,
    [0xf6] = &or_n,
    [0xf7] = &rst_30,
    [0xf8] = &ld_hl_sp_n,
    [0xf9] = &ld_sp_hl,
    [0xfa] = &ld_a_nn,
    [0xfb] = &ei,
    [0xfe] = &cp_n,
    [0xff] = &rst_38
};

static char *instNames[0x100] = {
    // 0x00 - 0x0f
    [0x00] = "NOP",
    [0x01] = "LD BC nn",
    [0x02] = "LD (BC) A",
    [0x03] = "INC BC",
    [0x04] = "INC B",
    [0x05] = "DEC B",
    [0x06] = "LD B n",
    [0x07] = "RLCA",
    [0x08] = "LD (nn) SP",
    [0x09] = "ADD HL BC",
    [0x0a] = "LD A (BC)",
    [0x0b] = "DEC BC",
    [0x0c] = "INC C",
    [0x0d] = "DEC C",
    [0x0e] = "LD C n",
    [0x0f] = "RRCA",

    // 0x10 - 0x1f
    [0x10] = "STOP 0",
    [0x11] = "LD DE nn",
    [0x12] = "LD (DE) A",
    [0x13] = "INC DE",
    [0x14] = "INC D",
    [0x15] = "DEC D",
    [0x16] = "LD D n",
    [0x17] = "RLA",
    [0x18] = "JR n",
    [0x19] = "ADD HL DE",
    [0x1a] = "LD A (DE)",
    [0x1b] = "DEC DE",
    [0x1c] = "INC E",
    [0x1d] = "DEC E",
    [0x1e] = "LD E n",
    [0x1f] = "RRA",

    // 0x20 - 0x2f
    [0x20] = "JR NZ n",
    [0x21] = "LD HL nn",
    [0x22] = "LD (HL+) A",
    [0x23] = "INC HL",
    [0x24] = "INC H",
    [0x25] = "DEC H",
    [0x26] = "LD H n",
    [0x27] = "DAA",
    [0x28] = "JR Z n",
    [0x29] = "ADD HL HL",
    [0x2a] = "LD A (HL+)",
    [0x2b] = "DEC HL",
    [0x2c] = "INC L",
    [0x2d] = "DEC L",
    [0x2e] = "LD L n",
    [0x2f] = "CPL",

    // 0x30 - 0x3f
    [0x30] = "JR NC n",
    [0x31] = "LD SP nn",
    [0x32] = "LD (HL-) A",
    [0x33] = "INC SP",
    [0x34] = "INC (HL)",
    [0x35] = "DEC (HL)",
    [0x36] = "LD (HL) n",
    [0x37] = "SCF",
    [0x38] = "JR C n",
    [0x39] = "ADD HL SP",
    [0x3a] = "LD A (HL-)",
    [0x3b] = "DEC SP",
    [0x3c] = "INC A",
    [0x3d] = "DEC A",
    [0x3e] = "LD A n",
    [0x3f] = "CCF",

    // 0x40 - 0x4f
    [0x40] = "LD B B",
    [0x41] = "LD B C",
    [0x42] = "LD B D",
    [0x43] = "LD B E",
    [0x44] = "LD B H",
    [0x45] = "LD B L",
    [0x46] = "LD B (HL)",
    [0x47] = "LD B A",
    [0x48] = "LD C B",
    [0x49] = "LD C C",
    [0x4a] = "LD C D",
    [0x4b] = "LD C E",
    [0x4c] = "LD C H",
    [0x4d] = "LD C L",
    [0x4e] = "LD C (HL)",
    [0x4f] = "LD C A",

    // 0x50 - 0x5f
    [0x50] = "LD D B",
    [0x51] = "LD D C",
    [0x52] = "LD D D",
    [0x53] = "LD D E",
    [0x54] = "LD D H",
    [0x55] = "LD D L",
    [0x56] = "LD D (HL)",
    [0x57] = "LD D A",
    [0x58] = "LD E B",
    [0x59] = "LD E C",
    [0x5a] = "LD E D",
    [0x5b] = "LD E E",
    [0x5c] = "LD E H",
    [0x5d] = "LD E L",
    [0x5e] = "LD E (HL)",
    [0x5f] = "LD E A",

    // 0x60 - 0x6f
    [0x60] = "LD H B",
    [0x61] = "LD H C",
    [0x62] = "LD H D",
    [0x63] = "LD H E",
    [0x64] = "LD H H",
    [0x65] = "LD H L",
    [0x66] = "LD H (HL)",
    [0x67] = "LD H A",
    [0x68] = "LD L B",
    [0x69] = "LD L C",
    [0x6a] = "LD L D",
    [0x6b] = "LD L E",
    [0x6c] = "LD L H",
    [0x6d] = "LD L L",
    [0x6e] = "LD L (HL)",
    [0x6f] = "LD L A",

    // 0x70 - 0x7f
    [0x70] = "LD (HL) B",
    [0x71] = "LD (HL) C",
    [0x72] = "LD (HL) D",
    [0x73] = "LD (HL) E",
    [0x74] = "LD (HL) H",
    [0x75] = "LD (HL) L",
    [0x76] = "HALT",
    [0x77] = "LD (HL) A",
    [0x78] = "LD A B",
    [0x79] = "LD A C",
    [0x7a] = "LD A D",
    [0x7b] = "LD A E",
    [0x7c] = "LD A H",
    [0x7d] = "LD A L",
    [0x7e] = "LD A (HL)",
    [0x7f] = "LD A A",

    // 0x80 - 0x8f
    [0x80] = "ADD A B",
    [0x81] = "ADD A C",
    [0x82] = "ADD A D",
    [0x83] = "ADD A E",
    [0x84] = "ADD A H",
    [0x85] = "ADD A L",
    [0x86] = "ADD A (HL)",
    [0x87] = "ADD A A",
    [0x88] = "ADC A B",
    [0x89] = "ADC A C",
    [0x8a] = "ADC A D",
    [0x8b] = "ADC A E",
    [0x8c] = "ADC A H",
    [0x8d] = "ADC A L",
    [0x8e] = "ADC A (HL)",
    [0x8f] = "ADC A A",

    // 0x90 - 0x9f
    [0x90] = "SUB B",
    [0x91] = "SUB C",
    [0x92] = "SUB D",
    [0x93] = "SUB E",
    [0x94] = "SUB H",
    [0x95] = "SUB L",
    [0x96] = "SUB (HL)",
    [0x97] = "SUB A",
    [0x98] = "SBC A B",
    [0x99] = "SBC A C",
    [0x9a] = "SBC A D",
    [0x9b] = "SBC A E",
    [0x9c] = "SBC A H",
    [0x9d] = "SBC A L",
    [0x9e] = "SBC A (HL)",
    [0x9f] = "SBC A A",

    // 0xa0 - 0xaf
    [0xa0] = "AND B",
    [0xa1] = "AND C",
    [0xa2] = "AND D",
    [0xa3] = "AND E",
    [0xa4] = "AND H",
    [0xa5] = "AND L",
    [0xa6] = "AND (HL)",
    [0xa7] = "AND A",
    [0xa8] = "XOR B",
    [0xa9] = "XOR C",
    [0xaa] = "XOR D",
    [0xab] = "XOR E",
    [0xac] = "XOR H",
    [0xad] = "XOR L",
    [0xae] = "XOR (HL)",
    [0xaf] = "XOR A",

    // 0xb0 - 0xbf
    [0xb0] = "OR B",
    [0xb1] = "OR C",
    [0xb2] = "OR D",
    [0xb3] = "OR E",
    [0xb4] = "OR H",
    [0xb5] = "OR L",
    [0xb6] = "OR (HL)",
    [0xb7] = "OR A",
    [0xb8] = "CP B",
    [0xb9] = "CP C",
    [0xba] = "CP D",
    [0xbb] = "CP E",
    [0xbc] = "CP H",
    [0xbd] = "CP L",
    [0xbe] = "CP (HL)",
    [0xbf] = "CP A",

    // 0xc0 - 0xcf
    [0xc0] = "RET NZ",
    [0xc1] = "POP BC",
    [0xc2] = "JP NZ nn",
    [0xc3] = "JP nn",
    [0xc4] = "CALL NZ nn",
    [0xc5] = "PUSH BC",
    [0xc6] = "ADD A n",
    [0xc7] = "RST 00H",
    [0xc8] = "RET Z",
    [0xc9] = "RET",
    [0xca] = "JP Z nn",
    [0xcb] = "PREFIX CB",
    [0xcc] = "CALL Z nn",
    [0xcd] = "CALL nn",
    [0xce] = "ADC A n",
    [0xcf] = "RST 08H",

    // 0xd0 - 0xdf
    [0xd0] = "RET NC",
    [0xd1] = "POP DE",
    [0xd2] = "JP NC nn",
    [0xd3] = NULL,
    [0xd4] = "CALL NC nn",
    [0xd5] = "PUSH DE",
    [0xd6] = "SUB n",
    [0xd7] = "RST 10H",
    [0xd8] = "RET C",
    [0xd9] = "RETI",
    [0xda] = "JP C nn",
    [0xdb] = NULL,
    [0xdc] = "CALL C nn",
    [0xdd] = NULL,
    [0xde] = "SBC A n",
    [0xdf] = "RST 18H",

    // 0xe0 - 0xef
    [0xe0] = "LDH (n) A",
    [0xe1] = "POP HL",
    [0xe2] = "LD (C) A",
    [0xe3] = NULL,
    [0xe4] = NULL,
    [0xe5] = "PUSH HL",
    [0xe6] = "AND n",
    [0xe7] = "RST 20H",
    [0xe8] = "ADD SP n",
    [0xe9] = "JP (HL)",
    [0xea] = "LD (nn) A",
    [0xeb] = NULL,
    [0xec] = NULL,
    [0xed] = NULL,
    [0xee] = "XOR n",
    [0xef] = "RST 28H",

    // 0xf0 - 0xff
    [0xf0] = "LDH A (n)",
    [0xf1] = "POP HL",
    [0xf2] = "LD A (C)",
    [0xf3] = "DI",
    [0xf4] = NULL,
    [0xf5] = "PUSH AF",
    [0xf6] = "OR n",
    [0xf7] = "RST 30H",
    [0xf8] = "LD HL SP+n",
    [0xf9] = "LD SP HL",
    [0xfa] = "LD A (nn)",
    [0xfb] = "EI",
    [0xfc] = NULL,
    [0xfd] = NULL,
    [0xfe] = "CP n",
    [0xff] = "RST 38H",
};

void step(gb_t *cpu) {
    uint8_t opcode = busRead8(cpu->regs.pc);
    if (instructions[opcode]) {
        if (cpu->interEnableBuffer) {
            cpu->interrupts = true;
            cpu->interEnableBuffer = false;
        }

        instructions[opcode](cpu);

        incAllTimers(numCycles);
        numCycles = 0;

        // if (instNames[opcode]) {
        //     char msg[128];
        //     sprintf(msg, "INST: %s OPCODE: %02x PC: %04x SP: %04x A: %02x BC: %04x DE: %04x HL: %04x F: %02x",
        //             instNames[opcode], opcode, cpu->regs.pc, cpu->regs.sp,
        //             cpu->regs.a, regBC(), regDE(), regHL(), cpu->regs.f);
        //     logMessage(msg, TRACE);
        // } else
        //     logMessage("Instruction Information Not Found", WARNING);
    } else {
        char msg[64];
        sprintf(msg, "Instruction not recognized INST: %x PC: %x", opcode, cpu->regs.pc);
        logMessage(msg, ERROR);
        exit(-1);
    }
}

