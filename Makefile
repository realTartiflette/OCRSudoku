CC = gcc

dependeciesImage = image.o openImage.o manipulatePixel.o grayscale.o blur.o threshold.o edgeDetector.o
dependeciesNN = network.o matrix.o neuralNetwork.o
dependeciesCut = cutting.o cut.o manipulatePixel.o

CFLAGSINTER = `pkg-config --cflags gtk+-3.0`
LIBSINTER = `pkg-config --libs gtk+-3.0`

CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image


all: image neuralNetwork solver cutting rotate interface

image: $(dependeciesImage)
	gcc $(dependeciesImage) $(LDLIBS) -o image -lm

interface: Interface/start.c
	gcc $(CFLAGSINTER) -o interface Interface/start.c $(LIBSINTER)

network: $(dependeciesNN)
	gcc $(dependeciesNN) -o network -lm

solver: solver.o
	gcc solver.o -o solver

cutting: ${dependeciesCut}
	gcc $(dependeciesCut) $(LDLIBS) -o cutting -lm

#image :
openImage: openImage.o
grayscale: grayscale.o manipulatePixel.o
manipulatePixel: manipulatePixel.o
blur: pixel_operations.o Blur.o
threshold: threshold.o manipulatePixel.o
edgeDetector: edgeDetector.o manipulatePixel.o

image.o: manipulateImage/openImage.h manipulateImage/grayscale.h
	gcc -c manipulateImage/image.c -lSDL2 -lSDL2main -lSDL2_image
openImage.o:
	gcc -c manipulateImage/openImage.c -lSDL2 -lSDL2main -lSDL2_image
grayscale.o: manipulateImage/manipulatePixel.h
	gcc -c manipulateImage/grayscale.c -lSDL2 -lSDL2main -lSDL2_image
manipulatePixel.o:
	gcc -c manipulateImage/manipulatePixel.c -lSDL2 -lSDL2main -lSDL2_image
blur.o :
	gcc -c manipulateImage/blur.c -lSDL2 -lSDL2main -lSDL2_image
threshold.o:
	gcc -c manipulateImage/threshold.c -lSDL2 -lSDL2main -lSDL2_image
edgeDetector.o:
	gcc -c manipulateImage/edgeDetector.c -lSDL2 -lSDL2main -lSDL2_image

#neural network

network.o: neuralNetwork/neuralNetwork.h
	gcc -c neuralNetwork/network.c
matrix.o: neuralNetwork/matrix.h
	gcc -c neuralNetwork/matrix.c
neuralNetwork.o: neuralNetwork/neuralNetwork.h
	gcc -c neuralNetwork/neuralNetwork.c

#solver
solver.o: solverSudoku/solver.c
	gcc -c solverSudoku/solver.c

#cutting

cutting.o: Cutting/cut.h
	gcc -c Cutting/cutting.c -lSDL2 -lSDL2main -lSDL2_image
cut.o: Cutting/manipulatePixel.h
	gcc -c Cutting/cut.c -lSDL2 -lSDL2main -lSDL2_image

#interface



clean:
	rm *.o results/* image network solver cutting interface



