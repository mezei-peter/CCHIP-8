#pragma once

#include <stdbool.h>
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
    bool pixels[SCREEN_WIDTH][SCREEN_HEIGHT];
} Screen;

Screen *scrn_new();
void scrn_free(Screen *scrn);
void scrn_clear(Screen *scrn);
bool scrn_get_px(Screen *scrn, size_t x, size_t y);
void scrn_draw_px(Screen *scrn, size_t x, size_t y, bool is_on);
