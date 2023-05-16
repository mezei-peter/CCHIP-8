#pragma once

#include <SDL2/SDL.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_PX_SIZE 15
#define COLOR_OFF 0, 0, 0, 255
#define COLOR_ON 255, 255, 255, 255

typedef struct Screen
{
    SDL_Window *window;
    SDL_Surface *surface;
} Screen;

Screen *scrn_new();
void scrn_free(Screen *scrn);
