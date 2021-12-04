#ifndef BIG_LINE_DETECTION_H
#define BIG_LINE_DETECTION_H

void maxHeight(SDL_Surface* img, int *maxYs, int *maxYe, int *maxX, int max);
int *Detection(SDL_Surface *img, int *isFailed);
void maxWidth(SDL_Surface* img, int *maxXs, int *maxXe, int *maxY, int max);
void DrawC(SDL_Surface *img, int x, int y1, int y2);
void DrawL(SDL_Surface *img, int x1, int x2, int y);
int checkCorner(int xs, int xe, int yl, int ys, int ye, int xc);


#endif