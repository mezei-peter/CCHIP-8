#pragma once

#include <stdint.h>
#include "memory.h"
#include "screen.h"
#include "keypad.h"
#include "binary_file.h"

#define PROGRAM_ADDR 0x200
#define FONTS_ADDR 0x050
#define FONT_SIZE 5

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
void cpu_free(Cpu *cpu);
void cpu_load_fonts(Cpu *cpu, Memory *mem);
void cpu_load_bin(Cpu *cpu, Memory *mem, BinaryFile *bin);
void cpu_cycle(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp);
