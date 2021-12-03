#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>

typedef struct matrix matrix;
struct matrix
{
    size_t rows;
    size_t cols;
    float *mat;
};

matrix *matAlloc(size_t rows, size_t cols);
void freeMat(matrix *M);

//prints a matrix on the standard output
void printMat(matrix *mat);

void copyMat(matrix *mat, matrix *res);

//add two matrices with the same dimentions
void addMat(matrix *A, matrix *B, matrix *result);

//multiplies two matrices with the cols of A == the rows of B
void multMat(matrix *A, matrix *B, matrix *result);

//transpose the matrix
void transMat(matrix *A, matrix *result);

void applyFunc(matrix *A, float (*opp) (float), matrix *result);

void fillMatWithRandom(matrix *mat);

#endif