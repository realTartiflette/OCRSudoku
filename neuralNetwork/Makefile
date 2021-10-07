CC = gcc
dependecies = main.o matrix.o neuralNetwork.o
main: $(dependecies)
	gcc $(dependecies) -o main
	rm $(dependecies)

main.o: neuralNetwork.h
matrix.o: matrix.h
neuralNetwork.o: neuralNetwork.h

clean: 
	rm main