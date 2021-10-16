#include <stdio.h> 
#include "matrix.h"

float getFloatRandom()
{
    return 2*((float)rand()/(float)(RAND_MAX)) - 1;
}

void isInRange(int rows, int cols, char *funcName)
{
    if(rows > MAX_ROWS || rows <= 0 || cols > MAX_COlS || cols <= 0)
    {
        printf("%s : rows or cols are to hight or < 0", funcName);
        exit(EXIT_FAILURE);
    }
}

void printMat(matrix mat)
{
    int rows = mat.rows, cols = mat.cols;
    isInRange(rows, cols, "printMat");

    for (int i = 0; i < rows; i++)
    {
        printf("| ");
        for (int j = 0; j < cols; j++)
            printf("%f ", mat.mat[i][j]);
        printf("|\n");
    }
    
}

void copyMat(matrix mat, matrix *res)
{
    int rows = mat.rows, cols = mat.cols;
    isInRange(rows, cols, "copyMat");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            res->mat[i][j] = mat.mat[i][j];
        }
    }

    res->rows = mat.rows, res->cols = mat.cols;
}

void addMat(matrix A, matrix B, matrix *result)
{
    int rowsA = A.rows, colsA = A.cols;
    int rowsB = B.rows, colsB = B.cols;

    isInRange(rowsA, colsA, "addMat");
    isInRange(rowsB, colsB, "addMat");

    if(rowsA != rowsB || colsA != colsB)
    {
        printf("addMat : rowsA != rowsB or colsA != colsB");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsA; j++)
            result->mat[i][j] = A.mat[i][j] + B.mat[i][j];
    }
    
    result->rows = A.rows, result->cols = A.cols;
}

void multMat(matrix A, matrix B, matrix *result)
{
    int rowsA = A.rows, colsA = A.cols;
    int rowsB = B.rows, colsB = B.cols;

    isInRange(rowsA, colsA, "multMat");
    isInRange(rowsB, colsB, "multMat");

    if(colsA != rowsB)
    {
        printf("addMat : rowsA != rowsB or colsA != colsB");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            float sum = 0;
            for (int k = 0; k < colsA; k++)
            {
                sum += A.mat[i][k]*B.mat[k][j];
            }
            result->mat[i][j] = sum;   
        }
    }

    result->rows = A.rows, result->cols = B.cols;
}

void transMat(matrix A, matrix *result)
{
    int rows = A.rows, cols = A.cols;
    isInRange(rows, cols, "transMat");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->mat[j][i] = A.mat[i][j];
        }
        
    }

    result->rows = A.cols, result->cols = A.rows;
    
}

void applyFunc(matrix A, float (*opp) (float), matrix *result)
{
    int rows = A.rows, cols = A.cols;
    isInRange(rows, cols, "applyFunc");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result->mat[i][j] = (*opp)(A.mat[i][j]);
        }
    }
    result->rows = A.rows, result->cols = A.cols;
}

void fillMatWithRandom(matrix *mat)
{
    int rows = mat->rows, cols = mat->cols;

    isInRange(rows, cols, "fillMatWithRandom");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            mat->mat[i][j] = getFloatRandom();
        }
    }
}