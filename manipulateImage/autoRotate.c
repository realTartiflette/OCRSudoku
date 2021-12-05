#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "manipulatePixel.h"

struct pos
{
	int a;
	int b;
};

struct pos* createPos(int a, int b)
{
	struct pos *Position;
	Position = malloc(sizeof (struct pos));
	Position->a = a;
	Position->b = b;
	return Position;
}

void GetPixelValue(SDL_Surface *img, int x, int y, Uint8 *color)
{
	Uint8 g, b;
	Uint32 pixel = GetPixel(img, x, y);
	SDL_GetRGB(pixel, img->format, color, &g, &b);
}

SDL_Surface* Rotate(SDL_Surface* img, double angRot)
{
	char* name = "results/rotatedIMG.jpg";
    int Xmid = img -> w / 2;
    int Ymid = img -> h / 2;
    angRot = (angRot*M_PI)/180;//converting degree to radian
    double sinAng = sin(angRot);
    double cosAng = cos(angRot);
    SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);//new rotated image

    for (int w = 0; w < img -> w; w++)
    {
        for (int h = 0; h < img -> h; h++)
        {
            int x = (int) ((w - Xmid) * cosAng - (h - Ymid) * sinAng + Xmid);
            int y = (int) ((w - Xmid) * sinAng + (h - Ymid) * cosAng + Ymid);
            
            if (x >= 0 && y >= 0 && x < img -> w && y < img -> h)
            {
                Uint32 pixel = GetPixel(img, x, y);
                PutPixel(newIMG, w, h, pixel);
            }
            else
            {
                Uint32 pixel = SDL_MapRGB(img -> format, 0, 0, 0);
                PutPixel(newIMG, w, h, pixel);
            }
        }
    }
    
    IMG_SaveJPG(newIMG, name, 100);
    return newIMG;
}

int *GetBlankLenght(SDL_Surface *img, int y)
{
	int* res = malloc(3*sizeof(int));
	int x1 = 0;
	int x2 = 0;
	int thereIsPattern = 0;
	int len = 0;
	int x=0;
	Uint8 color;
	while(!thereIsPattern && x<img->w)
	{
		GetPixelValue(img, x, y, &color);
		if(color > 150)
		{
			while(color>150 && x<img->w)
			{
				GetPixelValue(img, x, y, &color);
				x++;
			}
			if(x<img->w)
			{
				x1 = x-1;
				while(color<150 && x<img->w)
				{
					GetPixelValue(img, x, y, &color);
					len++;
					x++;
				}
				if (x<img->w)
				{
					x2 = x-1;
					thereIsPattern = 1;
					//printf("%i\n", len);
				}
				else
					len = 0;
			}
		}
		x++;
	}
	res[0]=len;
	res[1]=x1;
	res[2]=x2;
	return res;
}
double autoRotation(SDL_Surface *img)
{
    Uint8 color;
    double angle=0;
	//SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	struct pos *cornerPixel = createPos(-1,-1);
	
	for(int y = 0; y < img->h; y++)
	{
		for (int x = 0; x < img->w; x++)
		{
			GetPixelValue(img,x,y,&color);
			if(color>150)//consider white pixel
			{
				int tmpX = x;
				while(color>150 && tmpX+1<img->w)
				{
					tmpX++;
					GetPixelValue(img,tmpX,y,&color);
				}
				if(tmpX!=img->w && y+1<img->h)
				{
					//two while above are used to find the correct x of the corner
					cornerPixel = createPos((tmpX-x)/2+x-1, y);

					GetPixelValue(img,cornerPixel->a,y+1,&color);
					if(color<150)
					{
						int *res;
						int newLen = 0;
						int oldLen = -1;
						int i = 0;
						while(i<20 && (i+y+1)<img->h && newLen>oldLen)
						{
							oldLen = newLen;
							res = GetBlankLenght(img, (i+y+1));
							newLen = res[0];
							//printf("%i\n", newLen);
							i++;
						}
						if(i==20)
						{
							//printf("found angle %i %i\n",cornerPixel->a, cornerPixel->b);
							struct pos *leftPixel = createPos(res[1], y+i);
							struct pos *rightPixel = createPos(res[2], y+i);

							/*Uint32 pixel = SDL_MapRGB(img->format, 255, 0, 0);
							PutPixel(img, cornerPixel->a, cornerPixel->b, pixel);
							pixel = SDL_MapRGB(img->format, 255, 0, 0);
							PutPixel(img, leftPixel->a, leftPixel->b, pixel);
							pixel = SDL_MapRGB(img->format, 255, 0, 0);
							PutPixel(img, rightPixel->a, rightPixel->b, pixel);*/

							if((rightPixel->a-cornerPixel->a) > (cornerPixel->a - leftPixel->a))
							{
								//printf("atan((float)(%i - %i)/(float)((%i-%i)))*180/M_PI;\n", leftPixel->b, cornerPixel->b, rightPixel->a,cornerPixel->a);
								angle = atan((float)(leftPixel->b - cornerPixel->b)/(float)(rightPixel->a-cornerPixel->a))*180/M_PI;
							}
							else
							{
								//printf("atan((float)(%i - %i)/(float)((%i-%i)))*180/M_PI;\n", leftPixel->b, cornerPixel->b, cornerPixel->a,leftPixel->a);
								angle = -atan((float)(leftPixel->b - cornerPixel->b)/(float)(cornerPixel->a - leftPixel->a))*180/M_PI;
							}
							//printf("%f\n", angle);
							return angle;
						}
					}
					
				}
				else
					x = tmpX;
			}
		}
	}
	//printf("have not found an angle\n");
	return angle;
}