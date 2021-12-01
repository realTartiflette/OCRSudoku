#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_stdinc.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "edgeDetector.h"

void drawLine(SDL_Surface *img, int x1, int y1, int x2, int y2)
{
    
    if(x1 == x2)
    {
        for(int y = 0; y < img->h; y++)
        {
            Uint32 pixel = SDL_MapRGB(img->format, 255, 0, 0);
            PutPixel(img, x2, y, pixel);
        }
    }
    else
    {
        float coef = (y1 - y2)/(x1 - x2);
        float org = y1 - coef * x1;
        for(int x = 0; x < img->w; x++)
        {
            int y = coef * x + org;
            //printf("w:%d, h:%d\n", img->w, img->h);
            //printf("x:%d, y:%d\n", x, y);
            if(y < img->h && y > 0)
            {
                Uint32 pixel = SDL_MapRGB(img->format, 255, 0, 0);
                PutPixel(img, x, y, pixel);
            }
        }
    }
    
}



float getAveragePixel(SDL_Surface *img, point p)
{
    Uint8 r;
	Uint8 g;
	Uint8 b;
    int sum = 0;
    for (int i = p.x - 1; i <= p.x + 1; i++)
    {
        if(i >= 0 && i < img->w)
        {
            for(int j = p.y - 1; j <= p.y + 1; j++)
            {
                if(j >= 0 && j < img->h)
                {
                    Uint32 pixel = GetPixel(img, i, j);
                    SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    sum += g;
                }
            }
        }
        
    }

    return sum/9;
    
}

void getLine(SDL_Surface *img, point p)
{
    Uint8 r;
	Uint8 g;
	Uint8 b;

    int isLine = 0;

    for (int x = p.x - radius; x < p.x + radius; x++)
    {
        for(int y = p.y - radius; y < p.y + radius; y++)
        {
            float distTocenter = sqrt((x-p.x)*(x-p.x) + (y-p.y)*(y-p.y));
            if(distTocenter == radius)
            {
                Uint32 pixel = GetPixel(img, x, y);
                SDL_GetRGB(pixel, img->format, &r, &g, &b);
                if(g == 255 && r == 255 && b == 255)
                { 
                    
                    //float alpha = acos((p.x - p.y) / radius);
                    int diffX = (p.x - x), diffY = (p.y - y);
                    point np = {x, y};
                    int i = 0;
                    for (; i < 40 && getAveragePixel(img, np) >= 170; i++)
                    {
                        np.x += diffX;
                        np.y += diffY;
                        pixel = GetPixel(img, np.x, np.y);
			            SDL_GetRGB(pixel, img->format, &r, &g, &b);
                    }
                    

                    if(i == 40)
                    {
                        /*np.x = x;
                        np.y = y;
                        for (int j = 0; j < 35 && g == 255; j++)
                        {
                            np.x += diffX;
                            np.y += diffY;
                            pixel = SDL_MapRGB(img->format, 255, 0, 0);
                            PutPixel(img, np.x, np.y, pixel);
                        }*/

                        drawLine(img, p.x, p.y, np.x, np.y);
                    }
                }
            }
        }
    }
}

char* detectLine(char* path)
{
    SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/linesDetectedIMG.jpg";

    Uint8 r;
	Uint8 g;
	Uint8 b;
    for (int i = 0; i < img->w; i+=3)
    {
        for(int j = 0; j < img->h; j+=3)
        {
            Uint32 pixel = GetPixel(img, i, j);
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
            if(g == 255 && r == 255 && b == 255)
            {
                point p = {i,j};
                getLine(img, p);
            }
        }
    }
    
    IMG_SavePNG(img, name);
    return name;
}
