#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "cut.h"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		SDL_Surface* img = IMG_Load(argv[1]);
		CutGrid(img, 0,0,img->w -1,img->h -1);
		exit(EXIT_SUCCESS);
	}	

	exit(EXIT_FAILURE);
}
