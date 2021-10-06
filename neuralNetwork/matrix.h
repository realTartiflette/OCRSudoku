#ifndef MATRIX_H
#define MATRIX_H

#define MAX_ROWS 100
#define MAX_COlS 100

typedef float matrix[MAX_ROWS][MAX_COlS];

//prints a matrix on the standard output
void printMat(matrix mat, int rows, int cols);

//add two matrices with the same dimentions
void addMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result);

#endif