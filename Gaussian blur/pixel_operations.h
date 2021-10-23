#ifndef MANIPULATEPIXEL_H
#define MANIPULATEPIXEL_H

#include <stdlib.h>
#include <SDL2/SDL.h>

Uint32 GetPixel(SDL_Surface* surface, int x, int y);
void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

#endif