#pragma once

#include <stdbool.h>
#include "cpu.h"
#include "memory.h"
#include "screen.h"
#include "keypad.h"
#include "binary_file.h"

typedef struct System
{
    Cpu *cpu;
    Memory *memory;
    Screen *screen;
    Keypad *keypad;
} System;

System *sys_new(Cpu *cpu, Memory *memory, Screen *screen, Keypad *keypad);
void sys_free(System *sys);
int sys_run_bin(System *sys, BinaryFile *bin);
