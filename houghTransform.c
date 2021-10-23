#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include "manipulatePixel.h"

#define NUM 100

float DegToRad(int deg)
{
    return deg * (M_PI/180);
}

void HoughTransform(char path[], int** out)
{
    SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* namePNG = "blurMedianPNG.png";

    Uint8 r;
	Uint8 g;
	Uint8 b;

    int thetaDeg;
    float rho;
    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {
            Uint32 pixel = GetPixel(img, x, y);
            SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if ((r+g+b)/3 > 200)
            {
                for (thetaDeg = 0; thetaDeg < 180; thetaDeg++)
                {
                    float thetaRad = DegToRad(thetaDeg);
                    
                }
            }

        }
    }

    
}
