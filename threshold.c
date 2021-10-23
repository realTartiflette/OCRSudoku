#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h> 
#include <stdlib.h>
#include "manipulatePixel.h"


char* Threshold(char path[], int radius)
{
	SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* namePNG = "blurMedianPNG.png";
	
	int median[(img->w * img->h)/10];
	uint k = 0;
	

	unsigned int sum = 0;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint32 newPixel;

	for (int x = 0; x < img->w; x++)
	{
		
		for (int y = 0; y < img->h; y++)
		{
			if (k % 10 == 0)
			{
				Uint32 tmpPix = GetPixel(img, x, y);
				SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
				median[k/10] = (r+g+b)/3;
			}
			k++;
		}
	}
	mergeSort(median, 0, k/10);
	int seuil = median[k/20];

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
			if (average < (seuil + seuil/10))
				newPixel = SDL_MapRGB(img->format, 255, 255, 255);
			else
				newPixel = SDL_MapRGB(img->format, 0, 0, 0);
			
			sum = 0;
			PutPixel(newIMG, x, y, newPixel);
		}
	}
	
	

	IMG_SavePNG(newIMG, namePNG);
	return namePNG;
}

