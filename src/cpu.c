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
    cpu->clock = clock();
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

void cpu_draw(Cpu *cpu, Memory *mem, Screen *scrn)
{
    uint8_t y = cpu->var_regs[cpu->y] % SCREEN_HEIGHT;
    cpu->var_regs[VAR_REG_COUNT - 1] = 0;

    for (int i = 0; i < cpu->n; i++)
    {
        uint8_t x = cpu->var_regs[cpu->x] % SCREEN_WIDTH;
        uint8_t sprite_byte = mem_get_heap(mem, cpu->idx_reg + i);
        for (int j = 0; j < 8; j++)
        {
            bool bit = ((sprite_byte & 0x80) >> 7) == 1;
            bool px_bit = scrn_get_px(scrn, (size_t)x, (size_t)y);
            if (bit && px_bit)
            {
                scrn_draw_px(scrn, (size_t)x, (size_t)y, false);
                cpu->var_regs[VAR_REG_COUNT - 1] = 1;
            }
            if (bit && !px_bit)
            {
                scrn_draw_px(scrn, (size_t)x, (size_t)y, true);
            }
            if (x == SCREEN_WIDTH - 1)
            {
                break;
            }
            x++;
            sprite_byte <<= 1;
        }
        if (y == SCREEN_HEIGHT - 1)
        {
            break;
        }
        y++;
    }
}

void cpu_bcd_convert(Cpu *cpu, Memory *mem)
{
    uint8_t num = cpu->var_regs[cpu->x];
    uint8_t dig_3 = num % 10;
    num /= 10;
    uint8_t dig_2 = num % 10;
    uint8_t dig_1 = num / 10;
    mem_set_heap(mem, cpu->idx_reg, dig_1);
    mem_set_heap(mem, cpu->idx_reg + 1, dig_2);
    mem_set_heap(mem, cpu->idx_reg + 2, dig_3);
}

void cpu_execute(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp, Opcode opcode)
{
    bool overflow;
    switch (opcode)
    {
    case OPCODE_00E0:
        scrn_clear(scrn);
        break;
    case OPCODE_00EE:
        cpu->pc = mem_get_stack(mem, cpu->sp--);
        break;
    case OPCODE_0NNN:
        break;
    case OPCODE_1NNN:
        cpu->pc = cpu->nnn;
        break;
    case OPCODE_2NNN:
        mem_set_stack(mem, ++cpu->sp, cpu->pc);
        cpu->pc = cpu->nnn;
        break;
    case OPCODE_3XNN:
        if (cpu->nn == cpu->var_regs[cpu->x])
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_4XNN:
        if (cpu->nn != cpu->var_regs[cpu->x])
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_5XY0:
        if (cpu->var_regs[cpu->x] == cpu->var_regs[cpu->y])
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_6XNN:
        cpu->var_regs[cpu->x] = cpu->nn;
        break;
    case OPCODE_7XNN:
        cpu->var_regs[cpu->x] += cpu->nn;
        break;
    case OPCODE_8XY0:
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->y];
        break;
    case OPCODE_8XY1:
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->x] | cpu->var_regs[cpu->y];
        break;
    case OPCODE_8XY2:
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->x] & cpu->var_regs[cpu->y];
        break;
    case OPCODE_8XY3:
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->x] ^ cpu->var_regs[cpu->y];
        break;
    case OPCODE_8XY4:
        overflow = ((uint16_t)cpu->var_regs[cpu->x] + (uint16_t)cpu->var_regs[cpu->y]) > 0xFF;
        cpu->var_regs[cpu->x] = (uint8_t)cpu->var_regs[cpu->x] + (uint8_t)cpu->var_regs[cpu->y];
        if (overflow)
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 1;
        }
        else
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 0;
        }
        break;
    case OPCODE_8XY5:
        overflow = cpu->var_regs[cpu->x] < cpu->var_regs[cpu->y];
        cpu->var_regs[cpu->x] = (uint8_t)cpu->var_regs[cpu->x] - (uint8_t)cpu->var_regs[cpu->y];
        if (overflow)
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 0;
        }
        else
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 1;
        }
        break;
    case OPCODE_8XY6:
        // Using modern shift
        uint8_t lsb = cpu->var_regs[cpu->x] & 0x01;
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->x] >> 1;
        cpu->var_regs[VAR_REG_COUNT - 1] = lsb;

        break;
    case OPCODE_8XY7:
        overflow = cpu->var_regs[cpu->y] < cpu->var_regs[cpu->x];
        cpu->var_regs[cpu->x] = (uint8_t)cpu->var_regs[cpu->y] - (uint8_t)cpu->var_regs[cpu->x];
        if (overflow)
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 0;
        }
        else
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 1;
        }
        break;
    case OPCODE_8XYE:
        // Using modern shift
        uint8_t msb = (cpu->var_regs[cpu->x] & 0x80) >> 7;
        cpu->var_regs[cpu->x] = cpu->var_regs[cpu->x] << 1;
        cpu->var_regs[VAR_REG_COUNT - 1] = msb;
        break;
    case OPCODE_9XY0:
        if (cpu->var_regs[cpu->x] != cpu->var_regs[cpu->y])
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_ANNN:
        cpu->idx_reg = cpu->nnn;
        break;
    case OPCODE_BNNN:
        // Using classic jump with offset
        cpu->pc = cpu->nnn + (uint16_t)cpu->var_regs[0];
        break;
    case OPCODE_CXNN:
        srand(time(NULL));
        uint8_t r = rand() % 256;
        cpu->var_regs[cpu->x] = r & cpu->nn;
        break;
    case OPCODE_DXYN:
        cpu_draw(cpu, mem, scrn);
        break;
    case OPCODE_EX9E:
        if (keyp->keys[cpu->var_regs[cpu->x]].pressed)
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_EXA1:
        if (!keyp->keys[cpu->var_regs[cpu->x]].pressed)
        {
            cpu->pc += 2;
        }
        break;
    case OPCODE_FX07:
        cpu->var_regs[cpu->x] = cpu->dly_tmr;
        break;
    case OPCODE_FX0A:
        uint8_t pressed_key = keyp_get_pressed_key(keyp);
        if (pressed_key > 0xF)
        {
            cpu->key = pressed_key;
            if (!keyp->keys[cpu->key].pressed)
            {
                cpu->var_regs[cpu->x] = cpu->key;
                break;
            }
        }
        cpu->pc -= 2;
        break;
    case OPCODE_FX15:
        cpu->dly_tmr = cpu->var_regs[cpu->x];
        break;
    case OPCODE_FX18:
        cpu->snd_tmr = cpu->var_regs[cpu->x];
        break;
    case OPCODE_FX1E:
        // Using modern index addition
        cpu->idx_reg += (uint16_t)cpu->var_regs[cpu->x];
        if (cpu->idx_reg > MAX_INDEX)
        {
            cpu->idx_reg %= MAX_INDEX;
            cpu->var_regs[VAR_REG_COUNT - 1] = 1;
        }
        else
        {
            cpu->var_regs[VAR_REG_COUNT - 1] = 0;
        }
        break;
    case OPCODE_FX29:
        cpu->idx_reg = FONTS_ADDR + (cpu->var_regs[cpu->x] * FONT_SIZE);
        break;
    case OPCODE_FX33:
        cpu_bcd_convert(cpu, mem);
        break;
    case OPCODE_FX55:
        // Using modern memory storage
        for (int i = 0; i <= cpu->x; i++)
        {
            mem_set_heap(mem, cpu->idx_reg + i, cpu->var_regs[i]);
        }
        break;
    case OPCODE_FX65:
        // Using modern memory loading
        for (int i = 0; i <= cpu->x; i++)
        {
            cpu->var_regs[i] = mem_get_heap(mem, cpu->idx_reg + i);
        }
        break;
    default:
        break;
    }
}

void cpu_cycle(Cpu *cpu, Memory *mem, Screen *scrn, Keypad *keyp)
{
    if ((double)clock() - (double)cpu->clock > 1 / 60)
    {
        if (cpu->dly_tmr > 0)
        {
            cpu->dly_tmr--;
        }
        if (cpu->snd_tmr > 0)
        {
            cpu->snd_tmr--;
        }
        cpu->clock = clock();
    }
    uint16_t block = cpu_fetch(cpu, mem);
    Opcode opcode = cpu_decode(cpu, block);
    cpu_execute(cpu, mem, scrn, keyp, opcode);
}
