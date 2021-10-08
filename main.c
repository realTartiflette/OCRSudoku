#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "openImage.h"
#include "grayscale.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		
		//openImage(argv[1]);
		Grayscale(argv[1]);
		exit(EXIT_SUCCESS);

	}	

	exit(EXIT_FAILURE);
}
