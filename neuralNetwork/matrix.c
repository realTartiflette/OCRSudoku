#include <stdio.h> 
#include "matrix.h"

void isInRange(ulong rows, ulong cols, char *funcName)
{
    if(rows > MAX_ROWS || rows <= 0 || cols > MAX_COlS || cols <= 0)
    {
        printf("%s : rows or cols are to hight or < 0", funcName);
        exit(EXIT_FAILURE);
    }
}

void printMat(matrix mat, ulong rows, ulong cols)
{
    isInRange(rows, cols, "printMat");

    for (ulong i = 0; i < rows; i++)
    {
        printf("| ");
        for (ulong j = 0; j < cols; j++)
            printf("%f ", mat[i][j]);
        printf("|\n");
    }
    
}

void addMat(matrix A, ulong rowsA, ulong colsA, matrix B, ulong rowsB, ulong colsB, matrix result)
{
    isInRange(rowsA, colsA, "addMat");
    isInRange(rowsB, colsB, "addMat");

    if(rowsA != rowsB || colsA != colsB)
    {
        printf("addMat : rowsA != rowsB or colsA != colsB");
        exit(EXIT_FAILURE);
    }

    for (ulong i = 0; i < rowsA; i++)
    {
        for (ulong j = 0; j < colsA; j++)
            result[i][j] = A[i][j] + B[i][j];
    }
    
}

void multMat(matrix A, ulong rowsA, ulong colsA, matrix B, ulong rowsB, ulong colsB, matrix result)
{
    isInRange(rowsA, colsA, "multMat");
    isInRange(rowsB, colsB, "multMat");

    if(colsA != rowsB)
    {
        printf("addMat : rowsA != rowsB or colsA != colsB");
        exit(EXIT_FAILURE);
    }

    for (ulong i = 0; i < rowsA; i++)
    {
        for (ulong j = 0; j < colsB; j++)
        {
            float sum = 0;
            for (ulong k = 0; k < colsA; k++)
            {
                sum += A[i][k]*B[k][j];
            }
            result[i][j] = sum;   
        }
    }
}

void transMat(matrix A, ulong rows, ulong cols, matrix result)
{
    isInRange(rows, cols, "transMat");
    for (ulong i = 0; i < rows; i++)
    {
        for (ulong j = 0; j < cols; j++)
        {
            result[j][i] = A[i][j];
        }
        
    }
    
}

void applyFunc(matrix A, ulong rows, ulong cols, float (*opp) (float), matrix result)
{
    isInRange(rows, cols, "applyFunc");

    for (ulong i = 0; i < rows; i++)
    {
        for (ulong j = 0; j < cols; j++)
        {
            result[j][i] = (*opp)(A[i][j]);
        }
        
    }
}