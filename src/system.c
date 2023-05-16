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

bool sys_quit_event(System *sys)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return true;
            }
        default:
            break;
        }
    }
    return false;
}

int sys_run_bin(System *sys, BinaryFile *bin)
{
    int program_addr = mem_get_program_addr(sys->memory);
    for (int i = 0; i < bin->size; i++)
    {
        mem_set_heap(sys->memory, program_addr + i, bin->bytes[i]);
    }
    cpu_load_fonts(sys->cpu, sys->memory);

    bool running = true;
    while (running)
    {
        running = !sys_quit_event(sys);
    }
    return 0;
}
