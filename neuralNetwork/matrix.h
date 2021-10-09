#ifndef MATRIX_H
#define MATRIX_H

#define MAX_ROWS 100
#define MAX_COlS 100

#include <stdlib.h>

typedef float matrix[MAX_ROWS][MAX_COlS];

//prints a matrix on the standard output
void printMat(matrix mat, int rows, int cols);

void copyMat(matrix mat, int rows, int cols, matrix res);

//add two matrices with the same dimentions
void addMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result);

//multiplies two matrices with the cols of A == the rows of B
void multMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result);

//transpose the matrix
void transMat(matrix A, int rows, int cols, matrix result);

void applyFunc(matrix A, int rows, int cols, float (*opp) (float), matrix result);

#endif