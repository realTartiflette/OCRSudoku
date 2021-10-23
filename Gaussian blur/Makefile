# Simple SDL mini Makefile

CC=gcc

CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image

all:  main

main: pixel_operations.o Blur.o

Blur: pixel_operations.o Blur.o

pixel_operations.o : 
	gcc -c pixel_operations.c -lSDL2 -lSDL2main -lSDL2_image
Blur.o :
	gcc -c Blur.c -lSDL2 -lSDL2main -lSDL2_image

clean:
	${RM} *.o
	${RM} *.d
	${RM} Blur

# END
