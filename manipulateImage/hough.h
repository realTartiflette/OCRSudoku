#ifndef HOUGH_H
#define HOUGH_H

void DrawLine(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color);
char* houghTransform(char* path);

#endif
