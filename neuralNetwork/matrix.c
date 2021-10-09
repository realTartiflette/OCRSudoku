#include <stdio.h> 
#include "matrix.h"

void isInRange(int rows, int cols, char *funcName)
{
    if(rows > MAX_ROWS || rows <= 0 || cols > MAX_COlS || cols <= 0)
    {
        printf("%s : rows or cols are to hight or < 0", funcName);
        exit(EXIT_FAILURE);
    }
}

void printMat(matrix mat, int rows, int cols)
{
    isInRange(rows, cols, "printMat");

    for (int i = 0; i < rows; i++)
    {
        printf("| ");
        for (int j = 0; j < cols; j++)
            printf("%f ", mat[i][j]);
        printf("|\n");
    }
    
}

void copyMat(matrix mat, int rows, int cols, matrix res)
{
    isInRange(rows, cols, "copyMat");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            res[i][j] = mat[i][j];
        }
    }
}

void addMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result)
{
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
            result[i][j] = A[i][j] + B[i][j];
    }
    
}

void multMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result)
{
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
                sum += A[i][k]*B[k][j];
            }
            result[i][j] = sum;   
        }
    }
}

void transMat(matrix A, int rows, int cols, matrix result)
{
    isInRange(rows, cols, "transMat");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[j][i] = A[i][j];
        }
        
    }
    
}

void applyFunc(matrix A, int rows, int cols, float (*opp) (float), matrix result)
{
    isInRange(rows, cols, "applyFunc");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            result[i][j] = (*opp)(A[i][j]);
        }
        
    }
}