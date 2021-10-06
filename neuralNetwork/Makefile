CC = gcc
dependecies = main.o matrix.o
main: $(dependecies)
	gcc $(dependecies) -o main
	rm $(dependecies)

main.o: matrix.h
matrix.o: matrix.h

clean: 
	rm main