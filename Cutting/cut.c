#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include "manipulatePixel.h"
#include "../manipulateImage/big_line_detection.h"



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

SDL_Surface* RemovecontLeftAndRight(SDL_Surface*img)
{
	int h=img->h;
	int w=img->w;
	int point1=w/4;
	int point2=w/2;
	int point3=3*point1;
	int found1=0;
	int found2=0;
	int found3=0;
	int temp=img->h;
	int start=0;
	int end=0;
	int line=0;
	SDL_Surface* new_img=img;
	Uint8 r3;
	Uint8 g3;
	Uint8 b3;
	Uint8 r1;
	Uint8 g1;
	Uint8 b1;
	Uint8 r2;
	Uint8 g2;
	Uint8 b2;
	Uint8 rt;
	Uint8 bt;
	Uint8 gt;
	maxWidth(img,&start,&end,&line,img->w);
	if (line<temp/2 || line==0)
		line=temp;
	if (line>temp/3 && line<temp*(3/4))
		line==temp;
	while (h>temp/2 && found2==0 && found1==0 && found3==0)
	{
		Uint32 pixel1 = GetPixel(img, point1, h);
		Uint32 pixel2 = GetPixel(img, point2, h);
		Uint32 pixel3 = GetPixel(img, point3, h);
		SDL_GetRGB(pixel1, img->format, &r1, &g1, &b1);
		SDL_GetRGB(pixel2, img->format, &r2, &g2, &b2);
		SDL_GetRGB(pixel3, img->format, &r3, &g3, &b3);
		if (r2==255 )
		{
			if (h<temp-15)
			{
				found2=1;
				new_img=Cut(img,point1-8,1,point3+8,temp-1);
			}
			else if (h<line-10)
			{
				found2=1;
				new_img=Cut(img,point1-8,1,point3+8,temp-1);
			}			
		}
		
		else if (r1==255 && h<line-10)
		{
			found1=1;
			new_img=Cut(img,1,1,point2+16,temp-1);
		}
		else if (r3==255 && h<(line-10))
		{

			found3=1;
			new_img=Cut(img,point2-16,0,w-1,temp-1);
		}
		h--;

		if (found1==0 && found3==0 && found2==0 && h<temp+2/2)
		{
			new_img=Cut(img,point1-8,1,point3+8,temp-1);
		}


	}
	return new_img;
}

SDL_Surface* RemovecontTopAndBot(SDL_Surface*img)
{

	int h=img->h;
	int x=h-(img->w);
	int w=0;
	int point1=x/2;
	int pointc1=h/4;
	int point2=(h/2);
	int point3=h-(x/2);
	int pointc3=3*pointc1;
	int found1=0;
	int found2=0;
	int found3=0;
	int temp=img->w;
	int start=0;
	int end=0;
	int line=0;
	SDL_Surface* new_img=img;
	Uint8 r3;
	Uint8 g3;
	Uint8 b3;
	Uint8 r1;
	Uint8 g1;
	Uint8 b1;
	Uint8 r2;
	Uint8 g2;
	Uint8 b2;
	Uint8 rt;
	Uint8 bt;
	Uint8 gt;
	maxWidth(img,&start,&end,&line,img->w);
	if (line>pointc1+10)
		line=0;

	while ( found2==0 && found1==0 && found3==0)
	{
		Uint32 pixel1 = GetPixel(img, w, pointc1);
		Uint32 pixel2 = GetPixel(img, w, point2);
		Uint32 pixel3 = GetPixel(img, w, point3);
		SDL_GetRGB(pixel1, img->format, &r1, &g1, &b1);
		SDL_GetRGB(pixel2, img->format, &r2, &g2, &b2);
		SDL_GetRGB(pixel3, img->format, &r3, &g3, &b3);
		/*if (r2==255 && point2>line+20)
		{
			SDL_GetRGB(GetPixel(img,w+1,pointc3),img->format,&rt,&bt,&gt);
			if (rt!=255)
			{
				found2=1;
				new_img=Cut(img,0,point1,temp-1,point3);
			}		
		}*/
		if (r3==255 && pointc3>line+10) 
		{
			SDL_GetRGB(GetPixel(img,w+1,pointc3),img->format,&rt,&bt,&gt);
			if (rt==255)
			{
				found3=1;
				new_img=Cut(img,1,point1+(x/2)-5,temp-1,h-1);
			}
		}
		
		else if (r1==255 && pointc1>line+20)
		{
			SDL_GetRGB(GetPixel(img,w+1,point1),img->format,&rt,&bt,&gt);
			if (rt==255)
			{

				found1=1;
				new_img=Cut(img,1,1,temp-1,point3-(x/2)+5);
			}
		}

		w++;

		if (found1==0 && found3==0 && found2==0 && w>temp)
		{
			new_img=Cut(img,1,point1,temp-1,point3+5);
			found2=1;
		}


	}
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, temp+8,temp+8, 32, 0, 0, 0 ,0);

	for (int i = 0; i < new_img->w; ++i)
	{
		for (int j = 0; j < new_img->h; ++j)
		{
			Uint32 newpix=GetPixel(new_img,i,j);
			PutPixel(newIMG, i, j, newpix);
		}
	}

	return newIMG;
}


//call the Cut function 81 times
char **CutGrid(SDL_Surface* img,size_t x1,size_t y1,size_t x2, size_t y2)
{
	size_t plusx = (x2-x1)/9;
	size_t plusy = (y2-y1)/9;
	int x = 0;
    int y = 0;
	char **names = calloc(81, sizeof(char *));
	int k = 0;
    if(x2<img->w && y2 < img->h)
	{
		for(size_t i = x1;i<=x2-plusx && i<img->w;i+=plusx)
		{
            y = 0;
			for(size_t j = y1;j<=y2-plusy && j < img->h;j+=plusy)
			{
				
				names[k] = calloc(20 ,sizeof(char));
				strcpy(names[k], "results/0_0.jpeg");
				names[k][8] = x +48;
				names[k][10] = y +48;
                SDL_Surface* new_img = Cut(img,i,j,i+plusx,j+plusy);
				new_img = RemovecontLeftAndRight(new_img);
                new_img = RemovecontTopAndBot(new_img); 
				IMG_SaveJPG(new_img, names[k],100);
				SDL_FreeSurface(new_img);
                y++;
				k++;
			}
            x++;
		}
	}
    else
        printf("error in dimmention in CutGrid");
	return names;
}
