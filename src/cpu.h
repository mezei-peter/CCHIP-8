#pragma once

#include <stdint.h>
#include "memory.h"
#include "screen.h"
#include "keypad.h"
#include "binary_file.h"

#define PROGRAM_ADDR 0x200
#define FONTS_ADDR 0x050
#define FONT_SIZE 5

typedef enum
{
    OPCODE_0NNN,
    OPCODE_0E00,
    OPCODE_00EE,
    OPCODE_1NNN,
    OPCODE_2NNN,
    OPCODE_3XNN,
    OPCODE_4XNN,
    OPCODE_5XY0,
    OPCODE_6XNN,
    OPCODE_7XNN,
    OPCODE_8XY0,
    OPCODE_8XY1,
    OPCODE_8XY2,
    OPCODE_8XY3,
    OPCODE_8XY4,
    OPCODE_8XY5,
    OPCODE_8XY6,
    OPCODE_8XY7,
    OPCODE_8XYE,
    OPCODE_9XY0,
    OPCODE_ANNN,
    OPCODE_BNNN,
    OPCODE_CXNN,
    OPCODE_DXYN,
    OPCODE_EX9E,
    OPCODE_EXA1,
    OPCODE_FX07,
    OPCODE_FX0A,
    OPCODE_FX15,
    OPCODE_FX18,
    OPCODE_FX1E,
    OPCODE_FX29,
    OPCODE_FX33,
    OPCODE_FX55,
    OPCODE_FX65,
    OPCODE_INVALID
} Opcode;

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
