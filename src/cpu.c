#include <stdlib.h>

#include "cpu.h"

Cpu *cpu_new()
{
    Cpu *cpu = malloc(sizeof(Cpu));
    cpu->idx_reg = 0;
    cpu->dly_tmr = 0;
    cpu->snd_tmr = 0;
    cpu->pc = PROGRAM_ADDR;
    cpu->sp = 0;
    cpu->x = 0;
    cpu->y = 0;
    cpu->n = 0;
    cpu->nn = 0;
    cpu->nnn = 0;
    for (int i = 0; i < sizeof(cpu->var_regs) / sizeof(cpu->var_regs[0]); i++)
    {
        cpu->var_regs[i] = 0;
    }
    return cpu;
}

void cpu_free(Cpu *cpu)
{
    free(cpu);
}

void cpu_load_fonts(Cpu *cpu, Memory *mem)
{
    uint8_t fonts[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    for (int i = 0; i < sizeof(fonts) / sizeof(fonts[0]); i++)
    {
        mem_set_heap(mem, FONTS_ADDR + i, fonts[i]);
    }
}

void cpu_load_bin(Cpu *cpu, Memory *mem, BinaryFile *bin)
{
    for (int i = 0; i < bin->size; i++)
    {
        mem_set_heap(mem, PROGRAM_ADDR + i, bin->bytes[i]);
    }
}

uint16_t cpu_fetch(Cpu *cpu, Memory *mem)
{
    uint8_t byte_1 = mem->heap[cpu->pc];
    uint8_t byte_2 = mem->heap[cpu->pc + 1];
    uint16_t block = ((uint16_t)byte_1 << 8) + (uint16_t)byte_2;
    cpu->pc += 2;
    return block;
}

void cpu_cycle(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp)
{
    uint16_t block = cpu_fetch(cpu, mem);
}
