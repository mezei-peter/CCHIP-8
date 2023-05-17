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

uint8_t cpu_make_x(uint16_t block)
{
    return (uint8_t)((block & 0x0F00) >> 8);
}

uint8_t cpu_make_y(uint16_t block)
{
    return (((uint8_t)block) & 0xF0) >> 4;
}

uint8_t cpu_make_n(uint16_t block)
{
    return (((uint8_t)block) & 0x0F);
}

uint8_t cpu_make_nn(uint16_t block)
{
    return (uint8_t)block;
}

uint16_t cpu_make_nnn(uint16_t block)
{
    return block & 0x0FFF;
}

Opcode cpu_decode_0(Cpu *cpu, uint16_t block)
{
    if (block == 0x00EE)
    {
        return OPCODE_00EE;
    }
    if (block == 0x00E0)
    {
        return OPCODE_00E0;
    }
    cpu->nnn = cpu_make_nnn(block);
    return OPCODE_0NNN;
}

Opcode cpu_decode_8(Cpu *cpu, uint16_t block)
{
    //TODO
    return OPCODE_INVALID;
}

Opcode cpu_decode_e(Cpu *cpu, uint16_t block)
{
    //TODO
    return OPCODE_INVALID;
}

Opcode cpu_decode_f(Cpu *cpu, uint16_t block)
{
    //TODO
    return OPCODE_INVALID;
}

Opcode cpu_decode(Cpu *cpu, uint16_t block)
{
    uint8_t first_nibble = (uint8_t)(block >> 12);
    switch (first_nibble)
    {
    case 0:
        return cpu_decode_0(cpu, block);
    case 1:
        cpu->nnn = cpu_make_nnn(block);
        return OPCODE_1NNN;
    case 2:
        cpu->nnn = cpu_make_nnn(block);
        return OPCODE_2NNN;
    case 3:
        cpu->x = cpu_make_x(block);
        cpu->nn = cpu_make_nn(block);
        return OPCODE_3XNN;
    case 4:
        cpu->x = cpu_make_x(block);
        cpu->nn = cpu_make_nn(block);
        return OPCODE_4XNN;
    case 5:
        if ((block & 0x000F) != 0)
        {
            return OPCODE_INVALID;
        }
        cpu->x = cpu_make_x(block);
        cpu->y = cpu_make_y(block);
        return OPCODE_5XY0;
    case 6:
        cpu->x = cpu_make_x(block);
        cpu->nn = cpu_make_nn(block);
        return OPCODE_6XNN;
    case 7:
        cpu->x = cpu_make_x(block);
        cpu->nn = cpu_make_nn(block);
        return OPCODE_7XNN;
    case 8:
        return cpu_decode_8(cpu, block);
    case 9:
        if ((block & 0x000F) != 0)
        {
            return OPCODE_INVALID;
        }
        cpu->x = cpu_make_x(block);
        cpu->y = cpu_make_y(block);
        return OPCODE_9XY0;
    case 0xA:
        cpu->nnn = cpu_make_nnn(block);
        return OPCODE_ANNN;
    case 0xB:
        cpu->nnn = cpu_make_nnn(block);
        return OPCODE_BNNN;
    case 0xC:
        cpu->x = cpu_make_x(block);
        cpu->nn = cpu_make_nn(block);
        return OPCODE_CXNN;
    case 0xD:
        cpu->x = cpu_make_x(block);
        cpu->y = cpu_make_y(block);
        cpu->n = cpu_make_n(block);
        return OPCODE_DXYN;
    case 0xE:
        return cpu_decode_e(cpu, block);
    case 0xF:
        return cpu_decode_f(cpu, block);
    default:
        return OPCODE_INVALID;
    }
}

void cpu_cycle(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp)
{
    uint16_t block = cpu_fetch(cpu, mem);
}
