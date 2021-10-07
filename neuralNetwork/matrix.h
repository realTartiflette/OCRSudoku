#ifndef MATRIX_H
#define MATRIX_H

#define MAX_ROWS 100
#define MAX_COlS 100

#include <stdlib.h>

typedef float matrix[MAX_ROWS][MAX_COlS];

//prints a matrix on the standard output
void printMat(matrix mat, ulong rows, ulong cols);

//add two matrices with the same dimentions
void addMat(matrix A, ulong rowsA, ulong colsA, matrix B, ulong rowsB, ulong colsB, matrix result);

//multiplies two matrices with the cols of A == the rows of B
void multMat(matrix A, ulong rowsA, ulong colsA, matrix B, ulong rowsB, ulong colsB, matrix result);

//transpose the matrix
void transMat(matrix A, ulong rows, ulong cols, matrix result);

void applyFunc(matrix A, ulong rows, ulong cols, float (*opp) (float), matrix result);

#endif