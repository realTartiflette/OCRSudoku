#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>

void clean_resources(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t)
{
	if(t != NULL)
		SDL_DestroyTexture(t);

	if(r != NULL)
		SDL_DestroyRenderer(r);

	if(w != NULL)
		SDL_DestroyWindow(w);

	SDL_Quit();
}

int openImage(char path[])
{

	/* 
	 * Dans SDL, il y a tois grandes structures:
	 * - la fenetre: ce qui entoure l'affichage, on peut la deplacer la fermer etc
	 * - le rendu: ressemble a la feuille blanche dans laquelle on peut dessiner
	 * - la texture: la texture se place sur le rendu (dans notre cas se sera l'image que l' on veut afficher) 
	 */

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Surface *picture = NULL;
	SDL_Texture *texture = NULL;
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(NULL, NULL, NULL);
		exit(EXIT_FAILURE);
	}
       

	picture = IMG_Load(path);
	if(picture == NULL)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, renderer, NULL);
		exit(EXIT_FAILURE);
	}
	
	SDL_Rect dest_rect = {0, 0, picture->w, picture->h};


	window = SDL_CreateWindow("Affichage avec SDL2_Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, picture->w, picture->h, 0);
	if(window == NULL)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, NULL, NULL);
		exit(EXIT_FAILURE);
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if(renderer == NULL)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, NULL, NULL);
		exit(EXIT_FAILURE);
	}


	
	texture = SDL_CreateTextureFromSurface(renderer, picture);
	SDL_FreeSurface(picture);
	if(texture == NULL)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, renderer, NULL);
		exit(EXIT_FAILURE);
	}


	if(SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, renderer, texture);
		exit(EXIT_FAILURE);
	}

	if(SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0)
	{
		SDL_Log("ERREUR > %s\n", SDL_GetError());
		clean_resources(window, renderer, NULL);
		exit(EXIT_FAILURE);
	}

	SDL_RenderPresent(renderer);
	SDL_Delay(5000); //temps de l'execution du programme en ms

	clean_resources(window, renderer, texture);
	return EXIT_SUCCESS;

}
