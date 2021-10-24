CC = gcc
dependecies = main.o cut.o manipulatePixel.o
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image


all: main

main: $(dependecies)
	gcc $(dependecies) $(LDLIBS) -o main -lm
cut: cut.o manipulatePixel.o
manipulatePixel: manipulatePixel.o

main.o: cut.h
	gcc -c main.c -lSDL2 -lSDL2main -lSDL2_image
cut.o: manipulatePixel.h
	gcc -c cut.c -lSDL2 -lSDL2main -lSDL2_image
manipulatePixel.o:
	gcc -c manipulatePixel.c -lSDL2 -lSDL2main -lSDL2_image


clean:
	rm *.o results/* main


