#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h> 
#include <stdlib.h>
#include <math.h>
#include "manipulatePixel.h"
#include "hough.h"
#include "vector.h"
#include "edgeDetector.h"
#define VECTOR_INIT_CAPACITY 6
#define UNDEFINE  -1
#define SUCCESS 0
#define VECTOR_INIT(vec) vector vec;\
 vector_init(&vec)
//Store and track the stored data


struct couple 
{
    int x;
    int y;
};

struct couple_2
{
    struct couple x;
    struct couple y;
};



int HoughTransform(char path[],int threshold)  
{  
    SDL_Surface* img = IMG_Load(path);
    SDL_Surface* newIMG = img;
    int w = img->w;  
    int h = img->h;  
    unsigned char *data = (unsigned char*)(img->pixels);
    //Create the accu
    double hough_h;
    if (img->h>img->w)
    {
        hough_h =((sqrt(2.0) * (double)(img->h))/2.0);

    }
    else
    {
        hough_h =((sqrt(2.0) * (double)(img->w))/2.0);
    }
    int _accu_h = hough_h * 2.0; // -r -> +r  
    int _accu_w = 180;  
    unsigned int*_accu = (unsigned int*)calloc(_accu_h * _accu_w, sizeof(unsigned int));  
    int center_x = w/2;  
    int center_y = h/2;  
    for(int y=0;y<h;y++)  
    {  
        for(int x=0;x<w;x++)  
        {  
            if( data[ (y*w) + x] > 250 )  
            {  
               for(int t=0;t<180;t++)  
               { 
                   double r = ( ((double)x - center_x) * cos((double)t * (M_PI/180.0))) + (((double)y - center_y) * sin((double)t * (M_PI/180.0)));  
                  _accu[ (int)((round(r + hough_h) * 180.0)) + t]++;  
               }  
            }  
        }  
    }

    //Transform

    if(threshold == 0)
        threshold = 300;

    //Search the accumulator
    vector lines = GetLines(threshold,img,_accu_w,_accu_h,_accu,newIMG);

    //Draw the results
    printf("%d\n",vectorTotal(&lines) );
    for(int i=0;i<(vectorTotal(&lines));i++)
    {
        struct couple_2 *ligne=vectorGet(&lines,i);
        struct couple couplex=(*ligne).x;
        struct couple coupley=(*ligne).y;
        printf("%d %d\n",couplex.x,couplex.y );
        //printf("%d\n %d\n",coupley.x,coupley.y );

        //drawLine(newIMG,couplex.x,coupley.y,coupley.x,coupley.y);
    }
    free(_accu);
    IMG_SaveJPG(newIMG, "hough.jpg",100);   

    return 0;  
}  

vector GetLines(int threshold, SDL_Surface *img,int _accu_w,int _accu_h,unsigned int *_accu,SDL_Surface* newIMG)
{
    VECTOR_INIT(lines);
    if(_accu == 0)
        return lines;

    for(int r=0;r<_accu_h;r++)
    {
        for(int t=0;t<_accu_w;t++)
        {
            if((int)_accu[(r*_accu_w) + t] >= threshold)
            {
                //Is this point a local maxima (9x9)
                int max = _accu[(r*_accu_w) + t];
                for(int ly=-4;ly<=4;ly++)
                {
                    for(int lx=-4;lx<=4;lx++)
                    {
                        if( (ly+r>=0 && ly+r<_accu_h) && (lx+t>=0 && lx+t<_accu_w)  )
                        {
                            if( (int)_accu[( (r+ly)*_accu_w) + (t+lx)] > max )
                            {
                                max = _accu[( (r+ly)*_accu_w) + (t+lx)];
                                ly = lx = 5;
                            }
                        }
                    }
                }
                if(max > (int)_accu[(r*_accu_w) + t])
                    continue;


                int x1, y1, x2, y2;
                x1 = y1 = x2 = y2 = 0;

                if(t >= 45 && t <= 135)
                {
                    //y = (r - x cos(t)) / sin(t)
                    x1 = 0;
                    y1 = ((double)(r-(_accu_h/2)) - ((x1 - (img->w/2) ) * cos(t * M_PI/180.0))) / sin(t * M_PI/180.0) + (img->h / 2);
                    x2 = img->w - 0;
                    y2 = ((double)(r-(_accu_h/2)) - ((x2 - (img->w/2) ) * cos(t * M_PI/180.0))) / sin(t * M_PI/180.0) + (img->h / 2);
                }
                else
                {
                    //x = (r - y sin(t)) / cos(t);
                    y1 = 0;
                    x1 = ((double)(r-(_accu_h/2)) - ((y1 - (img->h/2) ) * sin(t * M_PI/180.0))) / cos(t * M_PI/180.0) + (img->w / 2);
                    y2 = img->h - 0;
                    x2 = ((double)(r-(_accu_h/2)) - ((y2 - (img->h/2) ) * sin(t * M_PI/180.0))) / cos(t * M_PI/180.0) + (img->w / 2);
                }
                struct couple couple1 =
                {
                    x1,
                    y1,
                };
                struct couple couple2 =
                {
                    x2,
                    y2,
                };
                struct couple_2 couple3 =
                {
                    couple1,
                    couple2,
                };
                //printf("%d %d\n",couple1.x,couple1.y );
                //printf("%d\n %d\n",couple2.x,couple2.y );

                struct couple couplex=couple3.x;
                struct couple coupley=couple3.y;
                drawLine(newIMG,couplex.x,coupley.y,coupley.x,coupley.y);


                //printf("%d %d\n",couplex.x,couplex.y );
                //printf("%d\n %d\n",coupley.x,coupley.y );

                vectorPushBack(&lines, &couple3);

            }
        }
    }
    return lines;
}

