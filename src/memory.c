#include "memory.h"
#include <stdlib.h>
#include <stdint.h>

Memory *mem_new()
{
    Memory *mem = malloc(sizeof(Memory));
    for (int i = 0; i < sizeof(mem->heap) / sizeof(mem->heap[0]); i++)
    {
        mem->heap[i] = 0;
    }
    for (int i = 0; i < sizeof(mem->stack) / sizeof(mem->stack[0]); i++)
    {
        mem->stack[i] = 0;
    }
    return mem;
}

void mem_free(Memory *mem)
{
    free(mem);
}

uint8_t mem_get_heap(Memory *mem, int i)
{
    return mem->heap[i];
}

void mem_set_heap(Memory *mem, int i, uint8_t val)
{
    mem->heap[i] = val;
}

void mem_set_stack(Memory *mem, int sp, uint16_t val)
{
    mem->stack[sp] = val;
}

uint16_t mem_get_stack(Memory *mem, int sp)
{
    return mem->stack[sp];
}

int mem_get_program_addr(Memory *mem)
{
    return PROGRAM_ADDR;
}
