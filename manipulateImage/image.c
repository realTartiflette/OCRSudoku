#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"
#include "blur.h"
#include "threshold.h"
//#include "edgeDetector.h"
#include "sobel.h"
#include "big_line_detection.h"
#include"../Cutting/cut.h"


int main(int argc, char **argv)
{
	if (argc == 2)
	{
    
		SDL_Surface *BaseImg = IMG_Load(argv[1]);
		SDL_Surface *grayImg = Grayscale(BaseImg);
		SDL_Surface *thresholdImg = Threshold(grayImg);
		SDL_Surface *blurImg = GaussianBlur(thresholdImg);
		SDL_Surface *sobelImg = sobel(blurImg);
		int isFailed = 0;
		int* res = Detection(sobelImg, &isFailed);
		CutGrid(thresholdImg, res[1], res[2], res[1]+res[0]-1, res[2]+res[0]-1);
		SDL_Surface *chiff = IMG_Load("results/8_0.jpeg");

		free(res);
		//name = detectLine(name);
		//name = houghTransform(name);

		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
