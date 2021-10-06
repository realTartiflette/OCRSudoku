#include <stdio.h> 
#include <stdlib.h>
#include "matrix.h"

void printMat(matrix mat, int rows, int cols)
{
    if(rows > MAX_ROWS || rows <= 0 || cols > MAX_COlS || cols <= 0)
    {
        printf("printMat : rows or cols are to hight or < 0");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++)
    {
        printf("| ");
        for (int j = 0; j < cols; j++)
            printf("%f ", mat[i][j]);
        printf("|\n");
    }
    
}

void addMat(matrix A, int rowsA, int colsA, matrix B, int rowsB, int colsB, matrix result)
{
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