CC = gcc

dependeciesImage = image.o openImage.o manipulatePixel.o grayscale.o blur.o threshold.o sobel.o edgeDetector.o big_line_detection.o cut.o matrix.o neuralNetwork.o initNetwork.o autoRotate.o
dependeciesNN = network.o matrix.o neuralNetwork.o initNetwork.o manipulatePixel.o
dependeciesInter = window.o solverSudoku.o openImage.o manipulatePixel.o grayscale.o blur.o threshold.o sobel.o edgeDetector.o big_line_detection.o cut.o matrix.o neuralNetwork.o initNetwork.o autoRotate.o

CFLAGSINTER = `pkg-config --cflags gtk+-3.0` -Wall -O3
LIBSINTER = `pkg-config --libs gtk+-3.0` -rdynamic `pkg-config --libs sdl2` -lSDL2_image

#CPPFLAGS= `pkg-config --cflags sdl2` -MMD
CGLAGS= -Wall -Wextra -Werror -std=c99 -03
LDFLAGS=
LDLIBS = `pkg-config --libs sdl2` -lSDL2_image


all: image network solver interface


image: $(dependeciesImage)
	gcc -g $(dependeciesImage) $(LDLIBS) -o image -lm

interface: $(dependeciesInter)
	gcc -g $(dependeciesInter) $(CFLAGSINTER) $(LIBSINTER) -o interface -lm

network: $(dependeciesNN)
	gcc $(dependeciesNN) $(LDLIBS) -o network -lm -g

solver: solver.o solverSudoku.o
	gcc -g solver.o solverSudoku.o -o solver

#image :
openImage: openImage.o
grayscale: grayscale.o manipulatePixel.o
manipulatePixel: manipulatePixel.o
blur: pixel_operations.o Blur.o
threshold: threshold.o manipulatePixel.o

edgeDetector: edgeDetector.o manipulatePixel.o
sobel: sobel.o manipulatePixel.o
big_line_detection: big_line_detection.o
cut: cut.o manipulatePixel.o
autoRotate: autoRotate.o manipulatePixel.o

image.o: manipulateImage/openImage.h manipulateImage/grayscale.h
	gcc -c -g manipulateImage/image.c -lSDL2 -lSDL2main -lSDL2_image
openImage.o:
	gcc -c -g manipulateImage/openImage.c -lSDL2 -lSDL2main -lSDL2_image
grayscale.o: manipulateImage/manipulatePixel.h
	gcc -c -g manipulateImage/grayscale.c -lSDL2 -lSDL2main -lSDL2_image
manipulatePixel.o:
	gcc -c -g manipulateImage/manipulatePixel.c -lSDL2 -lSDL2main -lSDL2_image
blur.o :
	gcc -c -g manipulateImage/blur.c -lSDL2 -lSDL2main -lSDL2_image
threshold.o:
	gcc -c -g manipulateImage/threshold.c -lSDL2 -lSDL2main -lSDL2_image
edgeDetector.o:
	gcc -c -g manipulateImage/edgeDetector.c -lSDL2 -lSDL2main -lSDL2_image
sobel.o:
	gcc -c -g manipulateImage/sobel.c -lSDL2 -lSDL2main -lSDL2_image
big_line_detection.o:
	gcc -c -g manipulateImage/big_line_detection.c -lSDL2 -lSDL2main -lSDL2_image
cut.o: Cutting/cut.h Cutting/manipulatePixel.h
	gcc -c Cutting/cut.c -lSDL2 -lSDL2main -lSDL2_image
autoRotate.o:
	gcc -c manipulateImage/autoRotate.c -lSDL2 -lSDL2main -lSDL2_image


#neural network

network.o: neuralNetwork/initNetwork.h
	gcc -c neuralNetwork/network.c -g
matrix.o: neuralNetwork/matrix.h
	gcc -c neuralNetwork/matrix.c -g
neuralNetwork.o: neuralNetwork/neuralNetwork.h
	gcc -c neuralNetwork/neuralNetwork.c -g
initNetwork.o: neuralNetwork/initNetwork.h manipulateImage/manipulatePixel.h
	gcc -c neuralNetwork/initNetwork.c -g -lSDL2 -lSDL2main -lSDL2_image


#solver
solver.o: solverSudoku/solver.c solverSudoku/solverSudoku.h
	gcc -g -c solverSudoku/solver.c
solverSudoku.o: solverSudoku/solverSudoku.h
	gcc -g -c solverSudoku/solverSudoku.c


#cutting

cutting.o: Cutting/cut.h
	gcc -c Cutting/cutting.c -lSDL2 -lSDL2main -lSDL2_image


#interface

window.o: Interface/window.h solverSudoku/solverSudoku.h
	gcc -g -c Interface/window.c $(CFLAGSINTER) $(LIBSINTER) -lSDL2 -lSDL2main -lSDL2_image


clean:
	rm *.o results/* image network solver interface error

