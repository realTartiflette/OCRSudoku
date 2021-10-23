#include <err.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "pixel_operations.h"

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
    const int initial_h = maximum(i - n, 0);
    const int initial_w = maximum(j - n, 0);
    const int final_h = minimum(i + n, surface->h - 1);
    const int final_w = minimum(j + n, surface->w - 1);
    const int nb_pixel = ((final_h - initial_h) * (final_w -initial_w));
    const Uint32 *p = surface->pixels;

    if (nb_pixel==0)
        {
            printf("nb:%d ih:%d iw:%d fh:%d fw:%d i:%d j:%d w:%d h:%d\n",nb_pixel,initial_h,initial_w,final_h,final_w,i,j,surface->w,surface->h);

        }



    Uint32 sum_r = 0, sum_g = 0, sum_b = 0;
    SDL_Color color;
    for (i = initial_h; i < final_h; i++)
        for(j = initial_w; j < final_w; j++)
        {
            Uint32 pixel = GetPixel(surface, i,j );
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


int main()
{
    SDL_Surface* image_surface;
    image_surface = IMG_Load("my_image.jpg");
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, image_surface->w, image_surface->h, 32, 0, 0, 0 ,0);

    int width = image_surface->w;
    int height = image_surface->h;
    for (int i=0;i<width;i++)
    {
	    for (int j=0;j<height;j++)
	    {
            Uint32 pixel = moyenne(image_surface, i, j, 5);
		    PutPixel(newIMG, i, j, pixel);
	    }
    }
    IMG_SavePNG(newIMG, "out.png");

    return 0;

}
