#include <stdio.h> 
#include <stdlib.h>
#include "matrix.h"

matrix *matAlloc(size_t rows, size_t cols)
{
    matrix *M = malloc(sizeof(matrix));
    M->rows = rows;
    M->cols = cols;
    M->mat = calloc(rows*cols, sizeof(float));
    return M;
}

void freeMat(matrix *M)
{
    free(M->mat);
    free(M);
}

float getFloatRandom()
{
    return 2*((float)rand()/(float)(RAND_MAX)) - 1;
}

void isInRange(size_t rows, size_t cols, char *funcName)
{
    if(rows <= 0 || cols <= 0)
    {
        printf("%s : rows (%ld) or cols(%ld) are <= 0 \n", funcName, rows, cols);
        exit(EXIT_FAILURE);
    }
}

void printMat(matrix *mat)
{
    size_t rows = mat->rows, cols = mat->cols;
    isInRange(rows, cols, "printMat");

    for (size_t i = 0; i < rows; i++)
    {
        printf("| ");
        for (size_t j = 0; j < cols; j++)
            printf("%0.2f ", mat->mat[i * cols + j]);
        printf("|\n");
    }
    
}

void copyMat(matrix *mat, matrix *res)
{
    size_t rows = mat->rows, cols = mat->cols;
    isInRange(rows, cols, "copyMat");
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            res->mat[i * cols + j] = mat->mat[i * cols + j];
        }
    }

    res->rows = mat->rows, res->cols = mat->cols;
}

void addMat(matrix *A, matrix *B, matrix *result)
{
    size_t rowsA = A->rows, colsA = A->cols;
    size_t rowsB = B->rows, colsB = B->cols;

    isInRange(rowsA, colsA, "addMat");
    isInRange(rowsB, colsB, "addMat");

    if(rowsA != rowsB || colsA != colsB)
    {
        printf("addMat : rowsA != rowsB or colsA != colsB \n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < rowsA; i++)
    {
        for (size_t j = 0; j < colsA; j++)
            result->mat[i * colsA + j] = A->mat[i * colsA + j] + B->mat[i * colsA + j];
    }
    
    result->rows = A->rows, result->cols = A->cols;
}

void multMat(matrix *A, matrix *B, matrix *result)
{
    size_t rowsA = A->rows, colsA = A->cols;
    size_t rowsB = B->rows, colsB = B->cols;

    isInRange(rowsA, colsA, "multMat");
    isInRange(rowsB, colsB, "multMat");

    if(colsA != rowsB)
    {
        printf("multMat : rowsA != rowsB or colsA != colsB\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < rowsA; i++)
    {
        for (size_t j = 0; j < colsB; j++)
        {
            float sum = 0;
            for (size_t k = 0; k < colsA; k++)
            {
                sum += A->mat[i * colsA + k]*B->mat[k * colsB + j];
            }
            result->mat[i * colsB + j] = sum;   
        }
    }

    result->rows = A->rows, result->cols = B->cols;
}

void transMat(matrix *A, matrix *result)
{
    size_t rows = A->rows, cols = A->cols;
    isInRange(rows, cols, "transMat");
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            result->mat[j * rows + i] = A->mat[i * cols + j];
        }
        
    }

    result->rows = A->cols, result->cols = A->rows;
    
}

void applyFunc(matrix *A, float (*opp) (float), matrix *result)
{
    size_t rows = A->rows, cols = A->cols;
    isInRange(rows, cols, "applyFunc");

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            result->mat[i * cols + j] = (*opp)(A->mat[i * cols + j]);
        }
    }
    result->rows = A->rows, result->cols = A->cols;
}

void fillMatWithRandom(matrix *mat)
{
    size_t rows = mat->rows, cols = mat->cols;

    isInRange(rows, cols, "fillMatWithRandom");

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            mat->mat[i * cols + j] = getFloatRandom();
        }
    }
}