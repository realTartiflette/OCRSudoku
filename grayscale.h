#ifndef GRAYSCALE_H
#define GRAYSCALE_H


Uint32 GetPixel(SDL_Surface* surface, int x, int y);
void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
char* Grayscale(char path[]);

#endif
