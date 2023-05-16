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
    cpu_load_fonts(sys->cpu, sys->memory);
    cpu_load_bin(sys->cpu, sys->memory, bin);

    bool running = true;
    while (running)
    {
        running = !sys_quit_event(sys);
    }
    return 0;
}
