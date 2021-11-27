#include <stdio.h>
#include <string.h>
#include "solverSudoku.h"



int matrix[9][9];

void print_sudoku()
{
    int i,j=0;
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            printf("%d\t",matrix[i][j]);
        }
        printf("\n\n");
    }
}

int number_unassigned(int *row, int *col)
{
    int num_unassign = 0;
    int i,j;
    for(i=0;i<9;i++)
    {
        for(j=0;j<9;j++)
        {
            if(matrix[i][j] == 0)
            {
                *row = i;
                *col = j;
                num_unassign = 1;
                return num_unassign;
            }
        }
    }
    return num_unassign;
}
    
int is_safe(int n, int r,int c)
{
    int i,j;
    for(i=0;i<9;i++)
    {
        if(matrix[r][i] == n)
            return 0;
    }
    for(i=0;i<9;i++)
    {
        if(matrix[i][c] == n)
            return 0;
    }
    int row_start = (r/3)*3;
    int col_start = (c/3)*3;
    for(i=row_start;i<row_start+3;i++)
    {
        for(j=col_start;j<col_start+3;j++)
        {
            if(matrix[i][j]==n)
                return 0;
        }
    }
    return 1;
}



int solve_sudoku()
{
    int row;
    int col;
    if(number_unassigned(&row, &col) == 0)
        return 1;
    int i;
    for(i=1;i<=9;i++)
    {
        if(is_safe(i, row, col))
        {
            matrix[row][col] = i;
            if(solve_sudoku())
                return 1;
            matrix[row][col]=0;
        }
    }
    return 0;
}




int solve(char *sudoku)
{
    FILE* fichier = NULL;
    fichier = fopen(sudoku, "r");

    char caractereActuel = ' ';

    if (fichier != NULL)
    {
        int i=0;
        int j=0;
        do
        {
            caractereActuel = fgetc(fichier); 
            if (j==9)
            {
                j=0;
                i=i+1;
            }

            if (caractereActuel=='.')
            {
                matrix[i][j]=0;
                j=j+1;
            }
            else if(caractereActuel=='1' || caractereActuel=='2'|| caractereActuel=='3'|| caractereActuel=='4'|| caractereActuel=='5'|| caractereActuel=='6'|| caractereActuel=='7'|| caractereActuel=='8'|| caractereActuel=='9')
            {
                matrix[i][j]=(int)caractereActuel-48;
                j=j+1;
            }



        } while (caractereActuel != EOF && i!=9); 

        fclose(fichier);
    



        if (solve_sudoku(matrix))
        {
            FILE* newfichier= fopen(strcat(sudoku,".result"),"w");
            for (int i = 0; i < 9; ++i)
            {
                if (i%3==0 && i!=0)
                {
                    fputc('\n',newfichier);
                }
                for (int j=0;j<9;j++)
                {
                    if (j%3==0)
                    {
                        fputc(' ',newfichier);
                    }
                    int n=matrix[i][j];
                    if (n==0)
                    {
                        fputc('.',newfichier);
                    }
                    else
                    {
                        caractereActuel=matrix[i][j]+48;
                        fputc(((char)caractereActuel),newfichier);
                    }
                }
                fputc('\n',newfichier);
            }
        }


        else
            printf("No solution\n");
        return 0;
    }
}



