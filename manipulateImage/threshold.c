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
	char* name = "results/thresholdIMG.jpg";
	
	/*int median[(img->w * img->h)/10];
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
	}*/

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

	if (((float)tier1/nbPixel) > 0.05)
		ref = 80;
	else if (((float)tier2/nbPixel) > 0.05)
		ref = 180; //110
	else if (((float)tier3/nbPixel) > 0.05)
		ref = 135;
	else if (((float)tier4/nbPixel) > 0.05)
		ref = 180;
	else 
		ref = 210;
	
	printf("%i", ref);
	
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
	return name;
}

