#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "hough.h"

char* houghTransform(char* path)
{
    SDL_Surface* img = IMG_Load(path);
	char* name = "results/hough.jpg";

    int width = img->w;
    int height = img->h;

    Uint8 r;
    Uint8 g;
    Uint8 b;

    int max_d = sqrt((width*width) + (height*height));
    int min_d = -1 * max_d;
    int max_theta = 180;
    int min_theta = 0;
    int threshold = 800;
    
    //int* accu = calloc(2*max_d*180, sizeof(int));
    int accu[4*max_d][180];
    for (int i = 0; i < 2*max_d; i++)
    {
        for (int j = 0; j < 180; j++)
        {
            accu[i][j] = 0;;
        }
    }

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
			SDL_GetRGB(GetPixel(img, x, y), img->format, &r, &g, &b);
			if (r == 255)
            {
                for (int theta = min_theta; theta < max_theta; theta++)
                {
                    int rho = x*cos(theta) + y*sin(theta);
                    rho += max_d;
                    
                    accu[rho][theta]++;
                }
            }
        }
    }

    for (int i = 0; i < 2*max_d; i++)
    {
        for (int theta = 0; theta < 180; theta++)
        {
            //printf("acc = %i\n", accu[i][theta]);
            if (accu[i][theta] > threshold)
            {
                float rho = i + min_d; 
                float a = cos(theta*(M_PI/180));
                float b = sin(theta*(M_PI/180));
                float x0 = (a*rho) + width/2;
                float y0 = (b*rho) + height/2;
                int x1 = x0+1000*(-b);
                int y1 = y0+1000*a;
                int x2 = x0-1000*(-b);
                int y2 = y0-1000*a;

                int dx = x2 - x1;
                int dy = y2 - y1;

                /*printf("a = %f\n", a);
                printf("b = %f\n", b);
                printf("x0 = %i\n", x0);
                printf("x1 = %i\n", x1);
                printf("dx = %i\n", dx);
                printf("dy = %i\n", dy);*/

                for (int x = x1; x < x2; x++)
                {
                    int y = y1+dy*(x-x1)/dx;
                    if (x < width && x > 0 && y < height && y > 0)
                    {
                        Uint32 averagePixel = SDL_MapRGB(img->format, 255, 0, 0);
			            PutPixel(img, x, y, averagePixel);
                    }
                    
                }
            }
            
        }
    }

    //for (int)
    IMG_SaveJPG(img, name, 100);
	return name;

}
