#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

	Uint8 R[radius * radius];
	Uint8 G[radius * radius];
	Uint8 B[radius * radius];

	Uint8 r;
	Uint8 g;
	Uint8 b;
	int cpt = 0;

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
						Uint8 tmpPixel = GetPixel(img, i, j);
						SDL_GetRGB(tmpPixel, img->format, &r, &g, &b);
						R[cpt] = r;
						G[cpt] = g;
						B[cpt] = b;

					}
					cpt++;

				}
			}
			cpt = 0;
			int size = sizeof(R) / sizeof(*R);
			qsort(R, size, sizeof(*R), comp);
			qsort(G, size, sizeof(*G), comp);	
			qsort(B, size, sizeof(*B), comp);
			
			r = R[size / 2];
			g = G[size / 2];
			b = B[size / 2];

			Uint32 newPixel = SDL_MapRGB(img->format, r, g, b);
			PutPixel(newIMG, x, y, newPixel);
		}
	}
	
	IMG_SavePNG(newIMG, namePNG);
	return namePNG;
}

