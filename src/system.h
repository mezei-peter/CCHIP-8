#pragma once

#include "cpu.h"
#include "memory.h"
#include "screen.h"
#include "keypad.h"

typedef struct System
{
    Cpu *cpu;
    Memory *memory;
    Screen *screen;
    Keypad *keypad;
} System;

System *sys_new(Cpu *cpu, Memory *memory, Screen *screen, Keypad *keypad);
