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

bool **scrn_pixels_init()
{
    bool **pixels = calloc(SCREEN_WIDTH, sizeof(bool *));
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        pixels[i] = (bool *)calloc(SCREEN_HEIGHT, sizeof(bool));
    }
    return pixels;
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
    scrn->pixels = scrn_pixels_init();
    scrn_pixels_clear(scrn);
    return scrn;
}

void scrn_free(Screen *scrn)
{
    SDL_DestroyWindow(scrn->window);
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        free(scrn->pixels[i]);
    }
    free(scrn->pixels);
    free(scrn);
}

void scrn_clear(Screen *scrn)
{
    SDL_Surface *surface = scrn->surface;
    SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, COLOR_OFF));
    scrn_pixels_clear(scrn);
    SDL_UpdateWindowSurface(scrn->window);
}
