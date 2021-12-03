#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "grayscale.h"
#include "blur.h"
#include "threshold.h"
#include "sobel.h"
#include "big_line_detection.h"
#include "../Cutting/cut.h"
#include "autoRotate.h"


int main(int argc, char **argv)
{
	if (argc == 2)
	{
		SDL_Surface *BaseImg = IMG_Load(argv[1]);
		SDL_Surface *grayImg = Grayscale(BaseImg);
		SDL_Surface *thresholdImg = Threshold(grayImg, 1);
		double angle = autoRotation(thresholdImg);
		SDL_Surface *rotatedImg = thresholdImg;
		if(angle!=0)
			rotatedImg = Rotate(thresholdImg,angle);
		SDL_Surface *blurImg = GaussianBlur(rotatedImg);
		SDL_Surface *sobelImg = sobel(blurImg);
		int* res = Detection(sobelImg);
        CutGrid(rotatedImg, res[1], res[2], res[1]+res[0]-1, res[2]+res[0]-1);
		free(res);
		free(BaseImg);
		free(grayImg);
		free(thresholdImg);
		free(rotatedImg);
		free(blurImg);
		free(sobelImg);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
