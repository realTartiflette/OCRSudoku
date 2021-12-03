#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "big_line_detection.h"

#define MAX(X, Y)  ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y)  ((X) < (Y) ? (X) : (Y))

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

int *Detection(SDL_Surface *img)
{
	char* name = "results/detectSquare.jpg";
    
    int maxLineXs = 0;
    int maxLineXe = 0;
    int maxLineY = 0;
    int maxColYs = 0;
    int maxColYe = 0;
    int maxColX = 0;

    int isDetected = 0;

    int maxw = img->w + 1;
    int maxh = img->h + 1;


    while (!isDetected)
    {

        if (maxLineXs == 0)
            maxWidth(img, &maxLineXs, &maxLineXe, &maxLineY, maxw);
        if (maxColYs == 0)
            maxHeight(img, &maxColYs, &maxColYe, &maxColX, maxh);

        int distLine = maxLineXe - maxLineXs;
        int distCol = maxColYe - maxColYs;

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
	int* res = malloc(3*sizeof(int)); //0: dist | 1: x | 2: y
	int p1 = abs(maxLineXs - maxColX) <= 10 && abs(maxColYs - maxLineY) <= 10;
    int p2 = abs(maxLineXs - maxColX) <= 10 && abs(maxColYe - maxLineY) <= 10;
    int p3 = abs(maxLineXe - maxColX) <= 10 && abs(maxColYs - maxLineY) <= 10;
    int p4 = abs(maxLineXe - maxColX) <= 10 && abs(maxColYe - maxLineY) <= 10;

	int distLine = maxLineXe - maxLineXs;
	int distCol = maxColYe - maxColYs;
	int maxDist =  MAX(distLine, distCol);
	res[0] = maxDist;

	if (p1)
	{
		res[1] = MIN(maxColX, maxLineXs);
		res[2] = MIN(maxColYs, maxLineY);
	}
	else if (p2)
	{
		res[1] = MIN(maxColX, maxLineXs);
		res[2] = MIN(maxColYe-distCol, maxLineY);
	}
	else if (p3)
	{
		res[1] = MIN(maxColX, maxLineXe-distLine);
		res[2] = MIN(maxColYs, maxLineY);
	}
	else
	{
		res[1] = MIN(maxColX, maxLineXe-distLine);
		res[2] = MIN(maxColYe-distCol, maxLineY);
	}

	
    IMG_SaveJPG(img, "results/bigLines.jpg", 100);
	return res;
}

