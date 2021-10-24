#ifndef MANIPULATEPIXEL_H
#define MANIPULATEPIXEL_H

#include <stdlib.h>
#include <SDL2/SDL.h>


void merge(int arr[], int l, int m, int r);
void mergeSort(int arr[], int l, int r);
Uint32 GetPixel(SDL_Surface* surface, int x, int y);
void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

#endif
