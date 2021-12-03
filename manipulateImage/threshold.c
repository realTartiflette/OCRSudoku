#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h> 
#include <stdlib.h>
#include "manipulatePixel.h"


SDL_Surface *Threshold(SDL_Surface *img, int radius)
{
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/thresholdIMG.jpg";

	int tier1 = 0;
	int tier2 = 0;
	int tier3 = 0;
	int tier4 = 0;
	int tier5 = 0;

	Uint8 r;
	Uint8 g;
	Uint8 b;

	int sum;
	Uint32 newPixel;

	for (int x = 0; x < img->w; x++)
	{
		
		for (int y = 0; y < img->h; y++)
		{
			Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			if (sum < 52)
				tier1++;
			else if (sum < 103)
				tier2++;
			else if (sum < 154)
				tier3++;
			else if (sum < 205)
				tier4++;
			else
				tier5++;
		}
	}


	float nbPixel = (float)(tier1+tier2+tier3+tier4+tier5);
	int ref;

	if (((float)tier1/nbPixel) > 0.06)
		ref = 80;
	else if (((float)tier2/nbPixel) > 0.06)
		ref = 110; 
	else if (((float)tier3/nbPixel) > 0.06)
		ref = 135;
	else if (((float)tier4/nbPixel) > 0.06)
		ref = 180;
	else 
		ref = 210;
	
	for (int x = 0; x < img->w; x++)
	{
		
		for (int y = 0; y < img->h; y++)
		{
			Uint32 tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
			sum = (r+g+b)/3;
			if (sum < ref)
				newPixel = SDL_MapRGB(img->format, 255, 255, 255);
			else
				newPixel = SDL_MapRGB(img->format, 0, 0, 0);

			PutPixel(newIMG, x, y, newPixel);
		}
	}
	

	IMG_SaveJPG(newIMG, name, 100);
	return newIMG;
}

