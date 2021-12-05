#ifndef CUT_H
#define CUT_H

char **CutGrid(SDL_Surface* ,size_t ,size_t ,size_t , size_t);
SDL_Surface* RemovecontTopAndBot(SDL_Surface*img);
SDL_Surface* RemovecontLeftAndRight(SDL_Surface*img);

#endif
