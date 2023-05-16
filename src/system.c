#include "system.h"

System *sys_new(Cpu *cpu, Memory *memory, Screen *screen, Keypad *keypad)
{
    System *system;
    system->cpu = cpu;
    system->memory = memory;
    system->screen = screen;
    system->keypad = keypad;
    return system;
}
