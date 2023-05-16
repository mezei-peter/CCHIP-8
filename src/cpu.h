#pragma once

#include <stdint.h>

typedef struct Cpu
{
    uint8_t var_regs[16];
    uint16_t idx_reg;
    uint8_t dly_tmr;
    uint8_t snd_tmr;
    uint16_t pc;
    uint8_t sp;
    uint8_t x;
    uint8_t y;
    uint8_t n;
    uint8_t nn;
    uint16_t nnn;
} Cpu;

Cpu *cpu_new();
