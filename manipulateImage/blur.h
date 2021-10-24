#ifndef BLUR_H
#define BLUR_H

#include "SDL2/SDL.h"


int minimum(int x,int y);
int maximum(int x,int y);
static Uint32 moyenne(SDL_Surface *surface, int i, int j, int n);
char* GaussianBlur(char path[]);

#endif