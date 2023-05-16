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
