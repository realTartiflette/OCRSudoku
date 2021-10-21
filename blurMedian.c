#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h> 
#include <stdlib.h>
#include "manipulatePixel.h"

int comp(const void* a, const void* b)
{
	return (*(int*)a - *(int*)b);
}

char* BlurMedian(char path[], int radius)
{
	SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* namePNG = "blurMedianPNG.png";

	unsigned int sum = 0;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint32 newPixel;

	for (int x = 0; x < img->w; x++)
	{
		for (int y = 0; y < img->h; y++)
		{
			for (int i = x - radius; i < x + radius; i++)
			{
				for (int j = y - radius; j < y + radius; j++)
				{
					if (i > 0 && i < img->w && j > 0 && j < img->h)
					{
						Uint32 tmpPixel = GetPixel(img, i, j);
						SDL_GetRGB(tmpPixel, img->format, &r, &g, &b);
						sum += r + g + b;

					}
				}
			}
			
			unsigned int average = sum / ((2*radius+1)*(2*radius+1));
			if (average < 65)
				newPixel = SDL_MapRGB(img->format, 0, 0, 0);
			else
				newPixel = SDL_MapRGB(img->format, 255, 255, 255);
			
			sum = 0;
			PutPixel(newIMG, x, y, newPixel);
		}
	}
	
	IMG_SavePNG(newIMG, namePNG);
	return namePNG;
}

