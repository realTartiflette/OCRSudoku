#include <stdio.h> 
#include "matrix.h"

float getFloatRandom()
{
    return 2*((float)rand()/(float)(RAND_MAX)) - 1;
}

void isInRange(size_t rows, size_t cols, char *funcName)
{
    if(rows > MAX_ROWS || rows <= 0 || cols > MAX_COlS || cols <= 0)
    {
        printf("%s : rows (%ld) or cols(%ld) are to hight or < 0 \n", funcName, rows, cols);
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
            printf("%0.2f ", mat->mat[i][j]);
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
            res->mat[i][j] = mat->mat[i][j];
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
            result->mat[i][j] = A->mat[i][j] + B->mat[i][j];
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
                sum += A->mat[i][k]*B->mat[k][j];
            }
            result->mat[i][j] = sum;   
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
            result->mat[j][i] = A->mat[i][j];
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
            result->mat[i][j] = (*opp)(A->mat[i][j]);
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
            mat->mat[i][j] = getFloatRandom();
        }
    }
}