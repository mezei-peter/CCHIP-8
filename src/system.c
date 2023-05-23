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

bool handle_input(System *sys)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            return true;
        case SDL_KEYDOWN:
            keyp_press_key(sys->keypad, event.key.keysym.scancode, true);
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            {
                return true;
            }
            break;
        case SDL_KEYUP:
            keyp_press_key(sys->keypad, event.key.keysym.scancode, false);
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
    binfile_free(bin);

    bool running = true;
    while (running)
    {
        cpu_cycle(sys->cpu, sys->memory, sys->screen, sys->keypad);
        running = !handle_input(sys);
    }
    return 0;
}
