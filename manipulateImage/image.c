#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"
#include "blur.h"
#include "threshold.h"
//#include "edgeDetector.h"
#include "hough.h"
#include "sobel.h"
#include "big_line_detection.h"
#include"../Cutting/cut.h"


int main(int argc, char **argv)
{
	if (argc == 2)
	{
		
		char* name = Grayscale(argv[1]);
		name = Threshold(name, 1);
		name = GaussianBlur(name);
		name = sobel(name);
		int* res = Detection(name);
        SDL_Surface* img = IMG_Load(name);
        CutGrid(img, res[1], res[2], res[1]+res[0]-1, res[2]+res[0]-1);
		free(res);
		//name = detectLine(name);
		//name = houghTransform(name);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
