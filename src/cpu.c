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
    uint8_t last_nibble = ((uint8_t)block) & 0x0F;
    cpu->x = cpu_make_x(block);
    cpu->y = cpu_make_y(block);
    cpu->n = cpu_make_n(block);
    switch (last_nibble)
    {
    case 0:
        return OPCODE_8XY0;
    case 1:
        return OPCODE_8XY1;
    case 2:
        return OPCODE_8XY2;
    case 3:
        return OPCODE_8XY3;
    case 4:
        return OPCODE_8XY4;
    case 5:
        return OPCODE_8XY5;
    case 6:
        return OPCODE_8XY6;
    case 7:
        return OPCODE_8XY7;
    case 0xE:
        return OPCODE_8XYE;
    default:
        return OPCODE_INVALID;
    }
}

Opcode cpu_decode_e(Cpu *cpu, uint16_t block)
{
    if ((block & 0xF0FF) == 0xE09E)
    {
        cpu->x = cpu_make_x(block);
        return OPCODE_EX9E;
    }
    if ((block & 0xF0FF) == 0xE0A1)
    {
        cpu->x = cpu_make_x(block);
        return OPCODE_EXA1;
    }
    return OPCODE_INVALID;
}

Opcode cpu_decode_f(Cpu *cpu, uint16_t block)
{
    cpu->x = cpu_make_x(block);
    uint8_t second_byte = (uint8_t)block;
    switch (second_byte)
    {
    case 0x07:
        return OPCODE_FX07;
    case 0x0A:
        return OPCODE_FX0A;
    case 0x15:
        return OPCODE_FX15;
    case 0x18:
        return OPCODE_FX18;
    case 0x1E:
        return OPCODE_FX1E;
    case 0x29:
        return OPCODE_FX29;
    case 0x33:
        return OPCODE_FX33;
    case 0x55:
        return OPCODE_FX55;
    case 0x65:
        return OPCODE_FX65;
    default:
        return OPCODE_INVALID;
    }
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

void cpu_execute(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp, Opcode opcode)
{
    switch (opcode)
    {
    case OPCODE_00E0:
        scrn_clear(scrn);
        break;
    case OPCODE_00EE:
        break;
    case OPCODE_0NNN:
        break;
    case OPCODE_1NNN:
        cpu->pc = cpu->nnn;
        break;
    case OPCODE_2NNN:
        break;
    case OPCODE_3XNN:
        break;
    case OPCODE_4XNN:
        break;
    case OPCODE_5XY0:
        break;
    case OPCODE_6XNN:
        cpu->var_regs[cpu->x] = cpu->nn;
        break;
    case OPCODE_7XNN:
        cpu->var_regs[cpu->x] += cpu->nn;
        break;
    case OPCODE_8XY0:
        break;
    case OPCODE_8XY1:
        break;
    case OPCODE_8XY2:
        break;
    case OPCODE_8XY3:
        break;
    case OPCODE_8XY4:
        break;
    case OPCODE_8XY5:
        break;
    case OPCODE_8XY6:
        break;
    case OPCODE_8XY7:
        break;
    case OPCODE_8XYE:
        break;
    case OPCODE_9XY0:
        break;
    case OPCODE_ANNN:
        cpu->idx_reg = cpu->nnn;
        break;
    case OPCODE_BNNN:
        break;
    case OPCODE_CXNN:
        break;
    case OPCODE_DXYN:
        break;
    case OPCODE_EX9E:
        break;
    case OPCODE_EXA1:
        break;
    case OPCODE_FX07:
        break;
    case OPCODE_FX0A:
        break;
    case OPCODE_FX15:
        break;
    case OPCODE_FX18:
        break;
    case OPCODE_FX1E:
        break;
    case OPCODE_FX29:
        break;
    case OPCODE_FX33:
        break;
    case OPCODE_FX55:
        break;
    case OPCODE_FX65:
        break;
    default:
        break;
    }
}

void cpu_cycle(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp)
{
    uint16_t block = cpu_fetch(cpu, mem);
    Opcode opcode = cpu_decode(cpu, block);
    cpu_execute(cpu, mem, scrn, keyp, opcode);
}
