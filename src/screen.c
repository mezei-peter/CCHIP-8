#include <stdlib.h>
#include <stdio.h>

#include "screen.h"

void scrn_pixels_clear(Screen *scrn)
{
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            scrn->pixels[i][j] = false;
        }
    }
}

Screen *scrn_new()
{

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        return NULL;
    }
    window = SDL_CreateWindow("CCHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH * SCREEN_PX_SIZE,
                              SCREEN_HEIGHT * SCREEN_PX_SIZE, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return NULL;
    }
    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, COLOR_OFF));
    SDL_UpdateWindowSurface(window);
    Screen *scrn = malloc(sizeof(Screen));
    scrn->window = window;
    scrn->surface = surface;
    scrn_pixels_clear(scrn);
    return scrn;
}

void scrn_free(Screen *scrn)
{
    SDL_DestroyWindow(scrn->window);
    free(scrn);
}

void scrn_clear(Screen *scrn)
{
    SDL_Surface *surface = scrn->surface;
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, COLOR_OFF));
    scrn_pixels_clear(scrn);
    SDL_UpdateWindowSurface(scrn->window);
}

bool scrn_get_px(Screen *scrn, size_t x, size_t y)
{
    return scrn->pixels[x][y];
}

void scrn_draw_px(Screen *scrn, size_t x, size_t y)
{
}
void scrn_erase_px(Screen *scrn, size_t x, size_t y)
{
}
