#ifndef BIG_LINE_DETECTION_H
#define BIG_LINE_DETECTION_H


int *Detection(SDL_Surface *img, int *isFailed);
void maxWidth(SDL_Surface* img, int *maxXs, int *maxXe, int *maxY, int max);
void maxHeight(SDL_Surface* img, int *maxYs, int *maxYe, int *maxX, int max);

#endif