#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>

char* BlurMedian(char path[], int radius)
{
	SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);

	for (int x = 0; x < img->w; x++)
	{
		for (int y = 0; y < img->h; y++)
		{
			
		}
	}

	
}

