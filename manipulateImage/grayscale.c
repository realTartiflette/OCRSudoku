#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "manipulatePixel.h"

SDL_Surface* Grayscale(SDL_Surface* img)
{
	char* name = "results/grayscaleIMG.jpg";
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);

  Uint8 r;
	Uint8 g;
	Uint8 b;

	for (int x = 0; x < img->w; x++)
	{
		for(int y = 0; y < img->h; y++)
		{
			Uint32 pixel = GetPixel(img, x, y);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			Uint8 gray = (Uint8)(r * 0.3 + g * 0.59 + b * 0.11);
			Uint32 averagePixel = SDL_MapRGB(img->format, gray, gray, gray);
			PutPixel(img, x, y, averagePixel);		
			
		}
	}

    IMG_SaveJPG(img, name, 100);
	//IMG_SavePNG(newIMG, name);
	return newIMG;
}
