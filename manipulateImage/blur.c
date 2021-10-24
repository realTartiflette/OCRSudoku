#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "manipulatePixel.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


int minimum(int x,int y)
{
    if (x>y)
    {
        return y;
    }
    else
        return x;
}
int maximum(int x,int y)
{
    if (x>y)
        return x;
    else 
        return y;
}


static Uint32 moyenne(SDL_Surface *surface, int i, int j, int n)
{
    const int initial_h = maximum(j - n, 0);
    const int initial_w = maximum(i- n, 0);
    const int final_h = minimum(j + n, surface->h - 1);
    const int final_w = minimum(i + n, surface->w - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w -initial_w));
    const Uint32 *p = surface->pixels;


    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;
    for (int x = initial_w; x < final_w; x++)
        for(int y = initial_h; y < final_h; y++)
        {
            Uint32 pixel = GetPixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
            sum_r += color.r;
            sum_g += color.g;
            sum_b += color.b;
        }



    if (nb_pixel==0)
    {
        return SDL_MapRGB(surface->format, 0, 0, 0);
    }
    else
        return SDL_MapRGB(surface->format, sum_r / nb_pixel, sum_g / nb_pixel, sum_b / nb_pixel);
}

char* GaussianBlur(char path[])
{
    SDL_Surface* img = IMG_Load(path);
	char* name = "results/blurIMG.jpg";

    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);

    int width = img->w;
    int height = img->h;
    for (int i=0;i<width;i++)
    {
	    for (int j=0;j<height;j++)
	    {
            Uint32 pixel = moyenne(img, i, j, 5);
		    PutPixel(newIMG, i, j, pixel);
	    }
    }
    IMG_SavePNG(newIMG, name);
	return name;
}

