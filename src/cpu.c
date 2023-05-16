#include <stdlib.h>

#include "cpu.h"

Cpu *cpu_new()
{
    Cpu *cpu = malloc(sizeof(Cpu));
    cpu->idx_reg = 0;
    cpu->dly_tmr = 0;
    cpu->snd_tmr = 0;
    cpu->pc = 0;
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
