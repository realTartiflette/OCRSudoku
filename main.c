#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"
#include "blurMedian.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		
		//openImage(argv[1]);
		//Grayscale(argv[1]);
		BlurMedian(argv[1], 3);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
