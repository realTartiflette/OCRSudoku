#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"
#include "blur.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
	
		char* name = Grayscale(argv[1]);
		GaussianBlur(name);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
