#include <stdio.h>
#include <stdlib.h>

#include "./logging/log.h"
#include "./include/interpreter.h"
#include "./include/bus.h"
#include "./include/cpu.h"

typedef void (*inst)(gb_t *);

// NOP

static void nop(gb_t *cpu) {
    cpu->regs.pc ++;
}

// 8-BIT LD

// Immediate 8 bits to register

static void ld_reg_n(uint8_t *val, gb_t *cpu) {
    *val = busRead8(++cpu->regs.pc);
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
    busWrite8(regHL(), busRead8(++cpu->regs.pc));
    cpu->regs.pc++;
}

// Register to Register

static void ld_b_b(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_b_c(gb_t *cpu) {
    cpu->regs.b = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_b_d(gb_t *cpu) {
    cpu->regs.b = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_b_e(gb_t *cpu) {
    cpu->regs.b = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_b_h(gb_t *cpu) {
    cpu->regs.b = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_b_l(gb_t *cpu) {
    cpu->regs.b = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_b_hl(gb_t *cpu) {
    cpu->regs.b = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_b_a(gb_t *cpu) {
    cpu->regs.b = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_c_b(gb_t *cpu) {
    cpu->regs.c = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_c_c(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_c_d(gb_t *cpu) {
    cpu->regs.c = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_c_e(gb_t *cpu) {
    cpu->regs.c = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_c_h(gb_t *cpu) {
    cpu->regs.c = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_c_l(gb_t *cpu) {
    cpu->regs.c = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_c_hl(gb_t *cpu) {
    cpu->regs.c = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_c_a(gb_t *cpu) {
    cpu->regs.c = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_d_b(gb_t *cpu) {
    cpu->regs.d = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_d_c(gb_t *cpu) {
    cpu->regs.d = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_d_d(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_d_e(gb_t *cpu) {
    cpu->regs.d = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_d_h(gb_t *cpu) {
    cpu->regs.d = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_d_l(gb_t *cpu) {
    cpu->regs.d = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_d_hl(gb_t *cpu) {
    cpu->regs.d = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_d_a(gb_t *cpu) {
    cpu->regs.d = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_e_b(gb_t *cpu) {
    cpu->regs.e = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_e_c(gb_t *cpu) {
    cpu->regs.e = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_e_d(gb_t *cpu) {
    cpu->regs.e = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_e_e(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_e_h(gb_t *cpu) {
    cpu->regs.e = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_e_l(gb_t *cpu) {
    cpu->regs.e = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_e_hl(gb_t *cpu) {
    cpu->regs.e = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_e_a(gb_t *cpu) {
    cpu->regs.e = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_h_b(gb_t *cpu) {
    cpu->regs.h = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_h_c(gb_t *cpu) {
    cpu->regs.h = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_h_d(gb_t *cpu) {
    cpu->regs.h = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_h_e(gb_t *cpu) {
    cpu->regs.h = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_h_h(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_h_l(gb_t *cpu) {
    cpu->regs.h = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_h_hl(gb_t *cpu) {
    cpu->regs.h = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_h_a(gb_t *cpu) {
    cpu->regs.h = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_l_b(gb_t *cpu) {
    cpu->regs.l = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_l_c(gb_t *cpu) {
    cpu->regs.l = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_l_d(gb_t *cpu) {
    cpu->regs.l = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_l_e(gb_t *cpu) {
    cpu->regs.l = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_l_h(gb_t *cpu) {
    cpu->regs.l = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_l_l(gb_t *cpu) {
    cpu->regs.pc++;
}

static void ld_l_hl(gb_t *cpu) {
    cpu->regs.l = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_l_a(gb_t *cpu) {
    cpu->regs.l = cpu->regs.a;
    cpu->regs.pc++;
}

static void ld_hl_b(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.b);
    cpu->regs.pc++;
}

static void ld_hl_c(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.c);
    cpu->regs.pc++;
}

static void ld_hl_d(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.d);
    cpu->regs.pc++;
}

static void ld_hl_e(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.e);
    cpu->regs.pc++;
}

static void ld_hl_h(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.h);
    cpu->regs.pc++;
}

static void ld_hl_l(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.l);
    cpu->regs.pc++;
}

static void halt(gb_t *cpu) {
    //TODO actualling implement this
    cpu->regs.pc++;
}

static void ld_hl_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    cpu->regs.pc++;
}

static void ld_a_b(gb_t *cpu) {
    cpu->regs.a = cpu->regs.b;
    cpu->regs.pc++;
}

static void ld_a_c(gb_t *cpu) {
    cpu->regs.a = cpu->regs.c;
    cpu->regs.pc++;
}

static void ld_a_d(gb_t *cpu) {
    cpu->regs.a = cpu->regs.d;
    cpu->regs.pc++;
}

static void ld_a_e(gb_t *cpu) {
    cpu->regs.a = cpu->regs.e;
    cpu->regs.pc++;
}

static void ld_a_h(gb_t *cpu) {
    cpu->regs.a = cpu->regs.h;
    cpu->regs.pc++;
}

static void ld_a_l(gb_t *cpu) {
    cpu->regs.a = cpu->regs.l;
    cpu->regs.pc++;
}

static void ld_a_hl(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    cpu->regs.pc++;
}

static void ld_a_a(gb_t *cpu) {
    cpu->regs.pc++;
}

// Load value into A

static void ld_a_bc(gb_t *cpu) {
    cpu->regs.a = busRead8(regBC());
    cpu->regs.pc++;
}

static void ld_a_de(gb_t *cpu) {
    cpu->regs.a = busRead8(regDE());
    cpu->regs.pc++;
}

static void ld_a_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc +=2;
    cpu->regs.a = busRead8(addr);
}

static void ld_a_hld(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    setHL(regHL() - 1);
    cpu->regs.pc++;
}

static void ld_a_hli(gb_t *cpu) {
    cpu->regs.a = busRead8(regHL());
    setHL(regHL() + 1);
    cpu->regs.pc++;
}

// This saves C + 0xff00 into A instead of saving C into A
static void ld_a_addr_c(gb_t *cpu) {
    uint16_t addr = 0xff00 | cpu->regs.c;
    cpu->regs.a = busRead8(addr);
    cpu->regs.pc++;
}

// This saves n + 0xff00 into A instead of saving n into A
static void ldh_a_n(gb_t *cpu) {
    uint16_t addr = 0xff00 | busRead8(++cpu->regs.pc);
    cpu->regs.a = busRead8(addr);
    cpu->regs.pc++;
}

// Load A into value

static void ld_bc_a(gb_t *cpu) {
    busWrite8(regBC(), cpu->regs.a);
    cpu->regs.pc++;
}

static void ld_de_a(gb_t *cpu) {
    busWrite8(regDE(), cpu->regs.a);
    cpu->regs.pc++;
}

static void ld_nn_a(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc += 2;
    busWrite8(addr, cpu->regs.a);
}

static void ld_hld_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    setHL(regHL() - 1);
    cpu->regs.pc++;
}

static void ld_hli_a(gb_t *cpu) {
    busWrite8(regHL(), cpu->regs.a);
    setHL(regHL() + 1);
    cpu->regs.pc++;
}

// This saves A into C + 0xff00 instead of saving A into C
static void ld_addr_c_a(gb_t *cpu) {
    uint16_t addr = 0xff00 | cpu->regs.c;
    busWrite8(addr, cpu->regs.a);
    cpu->regs.pc++;
}

static void ldh_n_a(gb_t *cpu) {
    uint16_t addr = 0xff00 | busRead8(++cpu->regs.pc);
    busWrite8(addr, cpu->regs.a);
    cpu->regs.pc++;
}

// 16-BIT LD

static void ld_bc_nn(gb_t *cpu) {
    setBC(busRead16(++cpu->regs.pc));
    cpu->regs.pc += 2;
}

static void ld_de_nn(gb_t *cpu) {
    setDE(busRead16(++cpu->regs.pc));
    cpu->regs.pc += 2;
}

static void ld_hl_nn(gb_t *cpu) {
    setHL(busRead16(++cpu->regs.pc));
    cpu->regs.pc += 2;
}

static void ld_sp_nn(gb_t *cpu) {
    cpu->regs.sp = busRead16(++cpu->regs.pc);
    cpu->regs.pc += 2;
}

static void ld_sp_hl(gb_t *cpu) {
    cpu->regs.sp = regHL();
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
}

static void ld_nn_sp(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    busWrite16(addr, cpu->regs.sp);
    cpu->regs.pc += 2;
}

// All stack push instructions

static void push(uint16_t val, gb_t *cpu) {
    cpu->regs.sp -= 2;
    busWrite16(cpu->regs.sp, val);
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
    cpu->regs.pc++;
}

static void add_a_a(gb_t *cpu) {
    add8(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void add_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    add8(val, cpu);
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
    cpu->regs.pc++;
}

static void adc_a_a(gb_t *cpu) {
    adc(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void adc_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    adc(val, cpu);
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
    cpu->regs.pc++;
}

static void sub_a(gb_t *cpu) {
    sub(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void sub_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    sub(val, cpu);
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
    cpu->regs.pc++;
}

static void sbc_a_a(gb_t *cpu) {
    sbc(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void sbc_a_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    sbc(val, cpu);
    cpu->regs.pc++;
}

// AND

static void and(uint8_t val, gb_t *cpu) {
    cpu->regs.a &= val;

    setN(false);
    setH(true);
    setC(false);
    setZ(cpu->regs.a == 0);
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
    cpu->regs.pc++;
}

static void and_a(gb_t *cpu) {
    and(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void and_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    and(val, cpu);
    cpu->regs.pc++;
}

// XOR

static void xor(uint8_t val, gb_t *cpu) {
    cpu->regs.a ^= val;
    setZ(cpu->regs.a == 0);
    setN(false);
    setH(false);
    setC(false);
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
    cpu->regs.pc++;
}

static void xor_a(gb_t *cpu) {
    xor(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void xor_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    xor(val, cpu);
    cpu->regs.pc++;
}

// OR

static void or(uint8_t val, gb_t *cpu) {
    cpu->regs.a |= val;

    setZ(cpu->regs.a == 0);
    setN(false);
    setH(false);
    setC(false);
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
    cpu->regs.pc++;
}

static void or_a(gb_t *cpu) {
    or(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void or_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    or(val, cpu);
    cpu->regs.pc++;
}

//CP

static void cp(uint8_t val, gb_t *cpu){
    setZ(cpu->regs.a == val);
    setN(true);
    setH((cpu->regs.a & 0xf) < (val & 0xf));
    setC(cpu->regs.a < val);
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
    cpu->regs.pc++;
}

static void cp_a(gb_t *cpu) {
    cp(cpu->regs.a, cpu);
    cpu->regs.pc++;
}

static void cp_n(gb_t *cpu) {
    uint8_t val = busRead8(++cpu->regs.pc);
    cp(val, cpu);
    cpu->regs.pc++;
}

// INC

static void inc(uint8_t *val, gb_t *cpu) {
    (*val)++;
    setZ(*val == 0);
    setN(false);
    setH((*val & 0xf) == 0);
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
    setHL(val);
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
    setHL(val);
}

static void dec_a(gb_t *cpu) {
    dec(&cpu->regs.a, cpu);
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
    int8_t  val = (int8_t)busRead8(++cpu->regs.pc);
    uint16_t temp = cpu->regs.sp;
    cpu->regs.sp += val;
    cpu->regs.pc++;

    temp ^= cpu->regs.sp ^ val;

    setZ(false);
    setN(false);
    setH(!!(temp & 0x10));
    setC(!!(temp & 0x100));
}

// 16 bit INC

static void inc_bc(gb_t *cpu) {
    setBC(regBC() + 1);
    cpu->regs.pc++;
}

static void inc_de(gb_t *cpu) {
    setDE(regDE() + 1);
    cpu->regs.pc++;
}

static void inc_hl(gb_t *cpu) {
    setHL(regHL() + 1);
    cpu->regs.pc++;
}

static void inc_sp(gb_t *cpu) {
    cpu->regs.sp++;
    cpu->regs.pc++;
}

// 16 Bit DEC

static void dec_bc(gb_t *cpu) {
    setBC(regBC() - 1);
    cpu->regs.pc++;
}

static void dec_de(gb_t *cpu) {
    setDE(regDE() - 1);
    cpu->regs.pc++;
}

static void dec_hl(gb_t *cpu) {
    setHL(regHL() - 1);
    cpu->regs.pc++;
}

static void dec_sp(gb_t *cpu) {
    cpu->regs.sp--;
    cpu->regs.pc++;
}

// JUMPS

// Jump to immediate 16 bits

static void jp_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    cpu->regs.pc = addr;
}

// Conditional Jumps

static void jp_nz_nn(gb_t *cpu) {
    if (!flagZ()) {
        jp_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void jp_z_nn(gb_t *cpu) {
    if (flagZ()) {
        jp_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void jp_nc_nn(gb_t *cpu) {
    if (!flagC()) {
        jp_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void jp_c_nn(gb_t *cpu) {
    if (flagC()) {
        jp_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

// Jump to address contained in HL

static void jp_hl(gb_t *cpu) {
    cpu->regs.pc = regHL();
}

// Relative Jumps

static void jr_n(gb_t *cpu) {
    int8_t jmpDiff = (int8_t)busRead8(++cpu->regs.pc);
    cpu->regs.pc++;
    cpu->regs.pc += jmpDiff;
}

// Relative Conditional Jumps

static void jr_nz_n(gb_t *cpu) {
    if (!flagZ()) {
        jr_n(cpu);
    } else {
        cpu->regs.pc++;
    }
}

static void jr_z_n(gb_t *cpu) {
    if (flagZ()) {
        jr_n(cpu);
    } else {
        cpu->regs.pc++;
    }
}

static void jr_nc_n(gb_t *cpu) {
    if (!flagC()) {
        jr_n(cpu);
    } else {
        cpu->regs.pc++;
    }
}

static void jr_c_n(gb_t *cpu) {
    if (flagC()) {
        jr_n(cpu);
    } else {
        cpu->regs.pc++;
    }
}

// CALLS

static void call_nn(gb_t *cpu) {
    uint16_t addr = busRead16(++cpu->regs.pc);
    push(cpu->regs.pc + 2, cpu);
    cpu->regs.pc = addr;
}

// Conditional Calls

static void call_nz_nn(gb_t *cpu) {
    if (!flagZ()) {
        call_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void call_z_nn(gb_t *cpu) {
    if (flagZ()) {
        call_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void call_nc_nn(gb_t *cpu) {
    if (!flagC()) {
        call_nn(cpu);
    } else {
        cpu->regs.pc += 3;
    }
}

static void call_c_nn(gb_t *cpu) {
    if (flagC()) {
        call_nn(cpu);
    } else {
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
}

static void ret_nz(gb_t *cpu) {
    if (!flagZ())
        ret(cpu);
    else
        cpu->regs.pc++;
}

static void ret_z(gb_t *cpu) {
    if (flagZ())
        ret(cpu);
    else
        cpu->regs.pc++;
}

static void ret_nc(gb_t *cpu) {
    if (!flagC())
        ret(cpu);
    else
        cpu->regs.pc++;
}

static void ret_c(gb_t *cpu) {
    if (flagC())
        ret(cpu);
    else
        cpu->regs.pc++;
}

static void reti(gb_t *cpu) {
    ret(cpu);
    // TODO Need to implement interrupts and implement them here, Will probably try to work 
    // on interupts next
}

// INTERUPTS

static void di(gb_t *cpu) {
    cpu->interrupts = false;
    cpu->regs.pc++;
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
    [0x08] = &ld_nn_sp,
    [0x09] = &add_hl_bc,
    [0x0a] = &ld_a_bc,
    [0x0b] = &dec_bc,
    [0x0c] = &inc_c,
    [0x0d] = &dec_c,
    [0x0e] = &ld_c_n,

    // 0x10 - 0x1f
    [0x11] = &ld_de_nn,
    [0x12] = &ld_de_a,
    [0x13] = &inc_de,
    [0x14] = &inc_d,
    [0x15] = &dec_d,
    [0x16] = &ld_d_n,
    [0x18] = &jr_n,
    [0x19] = &add_hl_de,
    [0x1a] = &ld_a_de,
    [0x1b] = &dec_de,
    [0x1c] = &inc_e,
    [0x1d] = &dec_e,
    [0x1e] = &ld_e_n,

    // 0x20 - 0x2f
    [0x20] = &jr_nz_n,
    [0x21] = &ld_hl_nn,
    [0x22] = &ld_hli_a,
    [0x23] = &inc_hl,
    [0x24] = &inc_h,
    [0x25] = &dec_h,
    [0x26] = &ld_h_n,
    [0x28] = &jr_z_n,
    [0x29] = &add_hl_hl,
    [0x2a] = &ld_a_hli,
    [0x2b] = &dec_hl,
    [0x2c] = &inc_l,
    [0x2d] = &dec_l,
    [0x2e] = &ld_l_n,

    // 0x30 - 0x3f
    [0x30] = &jr_nc_n,
    [0x31] = &ld_sp_nn,
    [0x32] = &ld_hld_a,
    [0x33] = &inc_sp,
    [0x34] = &inc_addr_hl,
    [0x35] = &dec_addr_hl,
    [0x36] = &ld_hl_n,
    [0x38] = &jr_c_n,
    [0x39] = &add_hl_sp,
    [0x3a] = &ld_a_hld,
    [0x3b] = &dec_sp,
    [0x3c] = &inc_a,
    [0x3d] = &dec_a,
    [0x3e] = &ld_a_n,

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
    // [0x76] = &halt,
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

void runInst(gb_t *cpu) {
    uint8_t opcode = busRead8(cpu->regs.pc);
    if (instructions[opcode]) {
        if (instNames[opcode]) {
            char msg[128];
            sprintf(msg, "INST: %s OPCODE: %02x PC: %04x SP: %04x A: %02x BC: %04x DE: %04x HL: %04x F: %02x",
                    instNames[opcode], opcode, cpu->regs.pc, cpu->regs.sp,
                    cpu->regs.a, regBC(), regDE(), regHL(), cpu->regs.f);
            logMessage(msg, TRACE);
        } else
            logMessage("Instruction Information Not Found", WARNING);

        instructions[opcode](cpu);
    } else {
        char msg[64];
        sprintf(msg, "Instruction not recognized INST: %x PC: %x", opcode, cpu->regs.pc);
        logMessage(msg, ERROR);
        exit(-1);
    }
}

