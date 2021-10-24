#ifndef OPENIMAGE_H
#define OPENIMAGE_H

#include "SDL2/SDL.h"

void clean_resources(SDL_Window* w, SDL_Renderer* r, SDL_Texture* t);
int openImage(char path[]);

#endif
