#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "big_line_detection.h"


int checkCorner(int xs, int xe, int yl, int ys, int ye, int xc)
{
    int p1 = abs(xs - xc) <= 10 && abs(ys - yl) <= 10;
    int p2 = abs(xs - xc) <= 10 && abs(ye - yl) <= 10;
    int p3 = abs(xe - xc) <= 10 && abs(ys - yl) <= 10;
    int p4 = abs(xe - xc) <= 10 && abs(ye - yl) <= 10;
    return p1 || p2 || p3 || p4;
}

void DrawL(SDL_Surface *img, int x1, int x2, int y)
{
    for (int x = x1; x<x2; x++)
    {
        PutPixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0));
    }
}

void DrawC(SDL_Surface *img, int x, int y1, int y2)
{
    for (int y = y1; y<y2; y++)
    {
        PutPixel(img, x, y, SDL_MapRGB(img->format, 255, 0, 0));
    }
}


void maxWidth(SDL_Surface* img, int *maxXs, int *maxXe, int *maxY, int max)
{
    Uint32 tmpPix;
    int tmpXs = 0;
    int tmpXe = 0;
    int tmpY = 0;
    Uint8 r, g, b; 

    for (int y = 0; y < img->h; y++)
    {
        for (int x = 0; x < img->w; x++)
        {
            tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            if (r >= 150)
            {
                tmpXs = x;
                tmpY = y;
                int isLine = 1;
                while (isLine)
                {
                    while (r >= 150 && x < img->w)
                    {
                        tmpPix = GetPixel(img, x, y);
                        SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
                        x++;
                    }
                    isLine = 0;
                    int i = -1;
                    int j = 0;
                    
                    while (x+j < img->w && j < 5 && !isLine)
                    {
                        while (y+i >= 0 && y+i < img->h && i < 2 && !isLine)
                        {
                            tmpPix = GetPixel(img, x+j, y+i);
                            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
                            if (r >= 150)
                                isLine = 1;
                            i++;
                        }
                        j++;
                    }
                    
                    if (isLine)
                    {
                        x += j;
                        y += i;
                    }
                        
                }
                tmpXe = x;
                if ((tmpXe-tmpXs) > (*maxXe-*maxXs) && (tmpXe-tmpXs) < max)
                {
                    *maxXs = tmpXs;
                    *maxXe = tmpXe;
                    *maxY = tmpY;
                }
            }
        }
    }
}

void maxHeight(SDL_Surface* img, int *maxYs, int *maxYe, int *maxX, int max)
{
    Uint32 tmpPix = 0;
    int tmpYs = 0;
    int tmpYe = 0;
    int tmpX = 0;
    Uint8 r, g, b; 

    for (int x = 0; x < img->w; x++)
    {
        for (int y = 0; y < img->h; y++)
        {
            tmpPix = GetPixel(img, x, y);
			SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
            if (r >= 150)
            {
                tmpYs = y;
                tmpX = x;
                int isCol = 1;
                while (isCol)
                {
                    while (r >= 150 && y < img->h)
                    {
                        tmpPix = GetPixel(img, x, y);
                        SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
                        y++;
                    }
                    isCol = 0;
                    int i = -1;
                    int j = 0;
                    
                    while (y+j < img->h && j < 5 && !isCol)
                    {
                        while (x+i >= 0 && x+i < img->w && i < 2 && !isCol)
                        {
                            tmpPix = GetPixel(img, x+i, y+j);
                            SDL_GetRGB(tmpPix, img->format, &r, &g, &b);
                            if (r >= 150)
                                isCol = 1;
                            i++;
                        }
                        j++;
                    }
                    if (isCol)
                    {
                        y += j;
                        x += i;
                    }
                        
                }
                tmpYe = y;
                if ((tmpYe-tmpYs) > (*maxYe-*maxYs) && (tmpYe-tmpYs) < max)
                {
                    *maxYs = tmpYs;
                    *maxYe = tmpYe;
                    *maxX = tmpX;
                }
            }
        }
    }
}

char* Detection(char* path)
{
    SDL_Surface* img = IMG_Load(path);
	SDL_Surface* newIMG = SDL_CreateRGBSurface(0, img->w, img->h, 32, 0, 0, 0 ,0);
	char* name = "results/detectSquare.jpg";
    
    int maxLineXs = 0;
    int maxLineXe = 0;
    int maxLineY = 0;
    int maxColYs = 0;
    int maxColYe = 0;
    int maxColX = 0;

    

    int isDetected = 0;

    int cpt = 0;
    int maxw = img->w + 1;
    int maxh = img->h + 1;


    while (!isDetected)
    {
        //printf("cpt = %i\n", cpt);
        cpt++;
        if (maxLineXs == 0)
            maxWidth(img, &maxLineXs, &maxLineXe, &maxLineY, maxw);
        if (maxColYs == 0)
            maxHeight(img, &maxColYs, &maxColYe, &maxColX, maxh);

        

        /*printf("maxLineXs = %i\n", maxLineXs);
        printf("maxLineXe = %i\n", maxLineXe);
        printf("maxLineY = %i\n", maxLineY);
        printf("maxColYs = %i\n", maxColYs);
        printf("maxColYe = %i\n", maxColYe);
        printf("maxColX = %i\n", maxColX);
        printf("-----------------------------------\n");*/


        int distLine = maxLineXe - maxLineXs;
        int distCol = maxColYe - maxColYs;

        printf("distLine = %i\n", distLine);
        printf("distCol = %i\n", distCol);

        if (abs(distLine-distCol) < 25 && checkCorner(maxLineXs, maxLineXe, maxLineY, maxColYs, maxColYe, maxColX))
        {
            isDetected = 1;
            DrawL(img, maxLineXs, maxLineXe, maxLineY);
            DrawC(img, maxColX, maxColYs, maxColYe);
        }
        else
        {
            if (distLine < 5 || distCol < 5)
            {
                isDetected = 1;
                printf("Grid detection failed\n");
            }
            else
            {
                if (distLine > distCol)
                {
                    maxw = distLine;
                    maxLineY = maxLineXs = maxLineXe = 0;
                }
                else
                {
                    maxh = distCol;
                    maxColX = maxColYs = maxColYe = 0;
                }
            
            }
        
        }

    }
    DrawL(img, maxLineXs, maxLineXe, maxLineY);
        DrawC(img, maxColX, maxColYs, maxColYe);
    
    IMG_SaveJPG(img, "results/bigLines.jpg", 100);
}

/*int crop_vertical_blob(SDL_Surface* img, int *border_1, int *border_2)
{
	Uint32 pixel;
	Uint8 r, g, b;
	int nb_inter = 0;
	int i = 0;
	int j = 0;

	//-----------------------------------draw empty lines------------------------------------
	SDL_Surface* copy = img;

	while (j < (img -> w))
	{
		while (i < (img -> h))
		{
			pixel = GetPixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0)
			{
				nb_inter += 1;
			}

			i += 1;
		}

		if (nb_inter > 10)
		{
			int a = 0;
			pixel = SDL_MapRGB(img -> format, 0, 255, 0);
			while(a < (img -> h))
			{
				PutPixel(copy, j, a, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		i = 0;
		j += 1;
	}


	//------------------------------------crop image----------------------------------------


	// detect green lines
	 
	int border1 = 0;
	int border2 = 0;

	int max = 0;
	int max_border1 = 0;
	int max_border2 = 0;

	j = 0;
	i = (img -> h) / 2;


	while (j < (img -> w))
	{
		pixel = GetPixel(copy, j, i);
		SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

		while (r == 0 && g == 0 && b == 0 && (j < (img -> w)))
		{
			pixel = GetPixel(copy, j, i);
			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
			j += 1;
		}

		if (r == 0 && g == 255 && b == 0)
		{
			border1 = j;

			while (r == 0 && g == 255 && b == 0 && (j < (img -> w)))
			{
				pixel = GetPixel(copy, j, i);
				SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
				j += 1;
			}

			border2 = j;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		j += 1;

	}



	*border_1 = max_border1;
	*border_2 = max_border2;


	// crop
	
	SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, max_border2 - max_border1, (copy -> h), 32, 0, 0, 0, 0);
	
	for (int i = 0; i < (copy -> h); i++)
	{
		for (int j = max_border1; j < max_border2; j++)
		{
			pixel = GetPixel(img, j, i);
			PutPixel(cropped_image, j - max_border1, i, pixel);
		}
	}

	*img = *cropped_image;

	
	free(cropped_image);
	free(copy);

	return 1;


}

int crop_horizontal_blob(SDL_Surface* img, int *border_1, int *border_2)
{
	Uint32 pixel;
	Uint8 r, g, b;
	int nb_inter = 0;
	int i = 0;
	int j = 0;

	-----------------------------------draw empty lines------------------------------------
	SDL_Surface* copy = img;

	while (i < (img -> h))
	{
		while (j < (img -> w))
		{
			pixel = GetPixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0)
			{
				nb_inter += 1;
			}

			j += 1;
		}

		if (nb_inter > 10)
		{
			int a = 0;
			pixel = SDL_MapRGB(img -> format, 0, 255, 0);
			while(a < (img -> w))
			{
				PutPixel(copy, a, i, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		j = 0;
		i += 1;
	}


	------------------------------------crop image----------------------------------------


	// detect green lines
	 
	int border1 = 0;
	int border2 = 0;

	int max = 0;
	int max_border1 = 0;
	int max_border2 = 0;

	i = 0;
	j = (img -> w) / 2;


	while (i < (img -> h))
	{
		pixel = GetPixel(copy, j, i);
		SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

		while (r == 0 && g == 0 && b == 0 && (i < (img -> h)))
		{
			pixel = GetPixel(copy, j, i);
			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
			i += 1;
		}

		if (r == 0 && g == 255 && b == 0)
		{
			border1 = i;

			while (r == 0 && g == 255 && b == 0 && (i < (img -> h)))
			{
				pixel = GetPixel(copy, j, i);
				SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
				i += 1;
			}
			
			border2 = i;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		i += 1;

	}

	*border_1 = max_border1;
	*border_2 = max_border2;


	SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, (copy -> w), max_border2 - max_border1, 32, 0, 0, 0, 0);
	
	for (int i = max_border1; i < max_border2; i++)
	{
		for (int j = 0; j < (img -> w); j++)
		{
			pixel = GetPixel(img, j, i);
			PutPixel(cropped_image, j, i - max_border1, pixel);
		}
	}

	*img = *cropped_image;

	free(cropped_image);
	free(copy);
	// crop successful return TRUE (1)

	return 1;
}

void DetectBiggestBlob(SDL_Surface *image_surface, int *pos_x, int *pos_y)
{
	int b1, b2;
	int b3, b4;
	int b5, b6;
	int b7, b8;

	crop_vertical_blob(image_surface, &b1, &b2);
	*pos_y = b1;
	crop_horizontal_blob(image_surface, &b3, &b4);
	*pos_x = b3;

	crop_vertical_blob(image_surface, &b5, &b6);
	if (b5 != b1)
		*pos_y += b5;

	crop_horizontal_blob(image_surface, &b7, &b8);
	if (b7 != b3)
		*pos_x += b7;

	while ((b1 != b5) || (b2 != b6) || (b3 != b7) || (b4 != b8))
	{
		crop_vertical_blob(image_surface, &b1, &b2);
		*pos_y += b1;
		crop_horizontal_blob(image_surface, &b3, &b4);
		*pos_x += b3;

		crop_vertical_blob(image_surface, &b5, &b6);
		if (b5 != b1)
			*pos_y += b5;
		crop_horizontal_blob(image_surface, &b7, &b8);
		if (b7 != b3)
			*pos_x += b7;
	}
	
	IMG_SaveJPG(image_surface, "results/blob.jpg", 100);

}*/

