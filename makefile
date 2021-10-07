CC = gcc
dependecies = main.o openImage.o
CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image


all: main

main: main.o openImage.o manipulatePixel.o grayscale.o blurMedian.o
openImage: openImage.o
grayscale: grayscale.o manipulatePixel.o
blurMedian: blurMedian.o manipulatePixel.o
manipulatePixel: manipulatePixel.o

main.o: openImage.h grayscale.h
	gcc -c main.c -lSDL2 -lSDL2main -lSDL2_image
openImage.o:
	gcc -c openImage.c -lSDL2 -lSDL2main -lSDL2_image
grayscale.o: manipulatePixel.h
	gcc -c grayscale.c -lSDL2 -lSDL2main -lSDL2_image
blurMedian.o:
	gcc -c blurMedian.c -lSDL2 -lSDL2main -lSDL2_image
manipulatePixel.o:
	gcc -c manipulatePixel.c -lSDL2 -lSDL2main -lSDL2_image


clean:
	rm *.o


