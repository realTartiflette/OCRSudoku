#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "manipulatePixel.h"



//x1,y1 top left angle
//x2,y2 bottom right angle
//return the image surface of the cut version
SDL_Surface* Cut(SDL_Surface* img, size_t x1,size_t y1,size_t x2, size_t y2)
{
	SDL_Surface* new_img = SDL_CreateRGBSurface(0, x2-x1, y2-y1, 32, 0, 0, 0, 0);
	if(x2<img->w && y2 < img->h)
	{
		for(size_t i = x1;i<x2;i++)
		{
			for(size_t j = y1;j<y2;j++)
			{
				//printf("%zu  %zu\n", i,j);
				PutPixel(new_img, i-x1, j-y1, GetPixel(img, i, j));
			}
		}
	}
	return new_img;
}

//call the Cut function 81 times
void CutGrid(SDL_Surface* img,size_t x1,size_t y1,size_t x2, size_t y2)
{
	size_t plusx = (x2-x1)/9;
	size_t plusy = (y2-y1)/9;
	if(x2<img->w && y2 < img->h)
	{
		for(size_t i = x1;i<=x2-plusx && i<img->w;i+=plusx)
		{
			for(size_t j = y1;j<=y2-plusy && j < img->h;j+=plusy)
			{
				char nameImg[]="results/0_0.jpeg";
				nameImg[8] = (i-x1)%11 +48;
				nameImg[10] = (j-y1)%11+48;
				SDL_Surface* new_img = Cut(img,i,j,i+plusx,j+plusy);
				IMG_SaveJPG(new_img, nameImg,100);
				SDL_FreeSurface(new_img);
			}
		}
	}
}