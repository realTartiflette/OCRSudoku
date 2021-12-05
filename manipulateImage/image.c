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
#include "../neuralNetwork/initNetwork.h"
#include "autoRotate.h"


int main(int argc, char **argv)
{
	if (argc == 2)
	{
    
		SDL_Surface *BaseImg = IMG_Load(argv[1]);
		SDL_Surface *grayImg = Grayscale(BaseImg);
		SDL_Surface *thresholdImg = Threshold(grayImg);
		double angle = autoRotation(thresholdImg);
		SDL_Surface *rotatedImg = Rotate(thresholdImg,angle);
        SDL_Surface *blurImg = GaussianBlur(rotatedImg);
		SDL_Surface *sobelImg = sobel(blurImg);

		int isFailed = 0;
		int* res = Detection(sobelImg, &isFailed);
		char **names = CutGrid(rotatedImg, res[1], res[2], res[1]+res[0]-1, res[2]+res[0]-1);
		
		neuralNetwork *nn = loadNetwork();
		for(size_t i = 0; i < 9; i++)
		{
			for (size_t j = 0; j < 9; j++)
			{
				matrix *inputs = matAlloc(1, 900);
				convertImageToMat(names[j*9+i], inputs);
				

				matrix *resN = matAlloc(1, 10);
				forwardPropagation(nn, inputs, resN);
				
				printf("%s : ", names[j*9+i]);
				printf("%d\n", getPrediction(resN));
				
				
				freeMat(inputs);
				freeMat(resN);
				free(names[j*9+i]);
			}
		}
		
		
		free(names);
		freeNetwork(nn);
		free(res);


		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
