CC = gcc
dependecies = main.o openImage.o
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image


all: main

main: main.o openImage.o
openImage: openImage.o

main.o: openImage.h
	gcc -c main.c -lSDL2 -lSDL2main -lSDL2_image
openImage.o:
	gcc -c openImage.c -lSDL2 -lSDL2main -lSDL2_image

clean:
	rm *.o


