CC = gcc
dependecies = main.o matrix.o neuralNetwork.o
CFLAGS = -Wall -Wextra -Werror -O3
main: $(dependecies)
	gcc $(dependecies) -o main -lm
	rm $(dependecies)

main.o: neuralNetwork.h
matrix.o: matrix.h
neuralNetwork.o: neuralNetwork.h

clean: 
	rm main