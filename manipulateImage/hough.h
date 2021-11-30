#ifndef HOUGH_H
#define HOUGH_H
#include "vector.h"


struct LineParameter{
    float angle;
    float distance;
};

void DrawLine(SDL_Surface *img, int x1, int y1, int x2, int y2);
int HoughTransform(char path[],int threshold);
vector GetLines(int threshold, SDL_Surface *img,int _accu_w,int _accu_h,unsigned int *_accu);



#endif
