#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "manipulatePixel.h"

char* Grayscale(char path[])
{
	SDL_Surface* img = IMG_Load(path);
	char* namePNG = "grayscalePNG.png";

	Uint8 r;
	Uint8 g;
	Uint8 b;

	for (int x = 0; x < img->w; x++)
	{
		for(int y = 0; y < img->h; y++)
		{
			Uint8 pixel = GetPixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			Uint8 gray = (Uint8)(r * 0.3f + g * 0.59f + b * 0.11f);
			Uint32 averagePixel = SDL_MapRGB(img->format, gray, gray, gray);
			PutPixel(img, x, y, averagePixel);		
			
		}
	}
	

	IMG_SavePNG(img, namePNG);
	return namePNG;
}
