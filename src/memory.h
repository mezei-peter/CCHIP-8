#pragma once

#include <stdint.h>

#define PROGRAM_ADDR 0x200
#define FONTS_ADDR 0x050
#define HEAP_SIZE 4096
#define STACK_SIZE 16

typedef struct Memory
{
    uint8_t heap[HEAP_SIZE];
    uint16_t stack[STACK_SIZE];
} Memory;

Memory *mem_new();
uint8_t mem_get_heap(Memory *mem, int i);
void mem_set_heap(Memory *mem, int i, uint8_t val);
void mem_set_stack(Memory *mem, int sp, uint16_t val);
uint16_t mem_get_stack(Memory *mem, int sp);
