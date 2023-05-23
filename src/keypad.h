#pragma once

#include <stdint.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct Key
{
    SDL_Scancode scancode;
    bool pressed;
} Key;

typedef struct Keypad
{
    Key keys[16];
} Keypad;

Keypad *keyp_new();
void keyp_free(Keypad *keyp);
