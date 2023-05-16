#pragma once

#include <stdint.h>

#define PROGRAM_ADDR 0x200
#define HEAP_SIZE (4096 - PROGRAM_ADDR)
#define STACK_SIZE 16

typedef struct Memory
{
    uint8_t heap[HEAP_SIZE];
    uint16_t stack[STACK_SIZE];
} Memory;

Memory *mem_new();
