#include <stdlib.h>

#include "system.h"

System *sys_new(Cpu *cpu, Memory *memory, Screen *screen, Keypad *keypad)
{
    System *system = malloc(sizeof(System));
    system->cpu = cpu;
    system->memory = memory;
    system->screen = screen;
    system->keypad = keypad;
    return system;
}

void sys_free(System *sys)
{
    cpu_free(sys->cpu);
    mem_free(sys->memory);
    scrn_free(sys->screen);
    keyp_free(sys->keypad);
    free(sys);
}

int sys_run_bin(System *sys, BinaryFile *bin)
{
    int program_addr = mem_get_program_addr(sys->memory);
    for (int i = 0; i < bin->size; i++)
    {
        mem_set_heap(sys->memory, program_addr + i, bin->bytes[i]);
    }
    return 0;
}
