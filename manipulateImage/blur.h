#ifndef BLUR_H
#define BLUR_H

#include "SDL2/SDL.h"


int minimum(int x,int y);
int maximum(int x,int y);
SDL_Surface *GaussianBlur(SDL_Surface *img);

#endif