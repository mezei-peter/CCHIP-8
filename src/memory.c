#include "memory.h"
#include <stdlib.h>

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
