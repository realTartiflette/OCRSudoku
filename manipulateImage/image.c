#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"
#include "blur.h"
#include "threshold.h"
#include "edgeDetector.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
	
		char* name = Grayscale(argv[1]);
		name = GaussianBlur(name);
		name = Threshold(name, 1);
		name = detectLine(name);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
