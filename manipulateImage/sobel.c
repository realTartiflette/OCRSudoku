#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "sobel.h"

Uint8 normalize(int Gx, int Gy)
{
    int n = sqrt(Gx*Gx + Gy*Gy);
    if (n > 255)
        n = 255;
    return (Uint8)n;
}

SDL_Surface *sobel(SDL_Surface *img)
{
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/sobelIMG.jpg";

    int sobel_x[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};

    int sobel_y[3][3] = {{-1, -2, -1},
                         {0, 0, 0},
                         {1, 2, 1}};
    
    Uint8 r, realR;
	Uint8 g, realG;
	Uint8 b, realB;
	Uint32 newPixel;

    int Gx_red, Gx_green, Gx_blue;
    int Gy_red, Gy_green, Gy_blue;

    for (int y = 0; y < img->h; y++)
	{
		for (int x = 0; x < img->w; x++)
		{
            Gx_red = Gx_green = Gx_blue = 0;
            Gy_red = Gy_green = Gy_blue = 0;

			for (int i = -1; i <= 1; i++)
			{
				for (int j = - 1; j <= 1; j++)
				{
					if (x+i > 0 && x+i < img->w && y+j > 0 && y+j < img->h)
					{
						Uint32 tmpPixel = GetPixel(img, x+i, y+j);
						SDL_GetRGB(tmpPixel, img->format, &r, &g, &b);
						
                        Gx_red += r * sobel_x[i+1][j+1];
                        Gx_green += g * sobel_x[i+1][j+1];
                        Gx_blue += b * sobel_x[i+1][j+1];

                        Gy_red += r * sobel_y[i+1][j+1];
                        Gy_green += g * sobel_y[i+1][j+1];
                        Gy_blue += b * sobel_y[i+1][j+1];
					}
				}
			}
            realR = normalize(Gx_red, Gy_red);
            realG = normalize(Gx_green, Gy_green);
            realB = normalize(Gx_blue, Gy_blue);
            newPixel = SDL_MapRGB(img->format, realR, realG, realB);
            PutPixel(newIMG, x, y, newPixel);
        }
    }

    IMG_SaveJPG(newIMG, name, 100);
	return newIMG;
}