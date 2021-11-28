#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

//if there is a NULL in matrix return 1
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
 
//verifying if n could be placed at [r][c]
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


//to solve the grid(return 1 if it couldn't solve it)
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
            matrix[row][col]=0;//backtracing
        }
    }
    return 0;
}

//check if there isn't a number twice in a row
int check_line_sudoku(int row)
{
    int res=1;
    int check[9]={0,0,0,0,0,0,0,0,0};
    for(int i=0;i<9;i++)
    {
        check[(matrix[row][i])-1]+=1;
    }
    int j=0;
    while(res==1 && j<9)
    {
        if (check[j]>=2)
        {
            res=0;
        }
        j++;
    }
    return res;
}

//check if there isn't a number twice in a col
int check_column_sudoku(int col)
{
    int res=1;
    int check[9]={0,0,0,0,0,0,0,0,0};
    for(int i=0;i<9;i++)
    {
        check[(matrix[i][col])-1]+=1;
    }
    int j=0;
    while(res==1 && j<9)
    {
        if (check[j]>1)
            res=0;
        j++;
    }
    return res;

}
//check if there isn't a number twice in a square
int check_square_sudoku(int row_start, int col_start)
{
    int check[9]={0,0,0,0,0,0,0,0,0};
    for(int i=row_start;i<row_start+3;i++)
    {
        for(int j=col_start;j<col_start+3;j++)
        {
            if(check[matrix[i][j]-1]!=0)
                return 0;
            check[matrix[i][j]-1]++;
        }
    }
    return 1;
}

//check if grid can be solved
int check_sudoku()
{
    int res=1;
    int i=0;
    while(res==1 && i<9)
    {
        res=check_line_sudoku(i);
        if(res==1)
            res=check_column_sudoku(i);
        if(res==1 && i%3==0)//check all squares
        {
            int j =0;
            while(res==1 && j<9)
            {
                res=check_square_sudoku(i,j);
                j+=3;
            }
        }
        i++;
    }
    return res;
}


void putText(char *src,char *dst)
{
    while(*src!=0)
    {
        *dst = *src;
        src++;
        dst++;
    }
    *dst =0;
}

char *solve(char *input)
{
    char *output = "";
    int res=1;
    char caractereActuel;

    if (*input != 0)
    {
        int i=0;
        int j=0;
        int nbElmt = 0;
        do
        {
            caractereActuel = *input;
            input++;
            if (j==9)
            {
                j=0;
                i=i+1;
            }

            if (caractereActuel=='.')
            {
                matrix[i][j]=0;
                nbElmt++;
                j=j+1;
            }
            else if(caractereActuel>='1' && caractereActuel<='9')
            {
                matrix[i][j]=(int)caractereActuel-48;
                nbElmt++;
                j=j+1;
            }
            else if (caractereActuel!=0 && caractereActuel!=' ' && caractereActuel!='\n')
            {
                res=0;
            }
        } while (caractereActuel != 0 && nbElmt<=81 && i!=9 && res==1);

        if(res==1 && nbElmt!=81)
            res=0;
        //copy all information into matrix if possible

        if (res == 1 && solve_sudoku())//if the file is correct and the solve algorithm is finished
        {
            res = check_sudoku();//chek if the sudoku is correct
            if (res==1)
            {
                output = (char *)malloc(111);
                char *tmp = output;
                for (int i = 0; i < 9; ++i)
                {
                    if (i%3==0 && i!=0)
                    {
                        *tmp = '\n';
                        tmp++;
                    }
                    for (int j=0;j<9;j++)
                    {
                        if (j%3==0)
                        {
                            *tmp = ' ';
                            tmp++;
                        }
                        int n=matrix[i][j];
                        if (n==0)
                        {
                            *tmp = '.';
                            tmp++;
                        }
                        else
                        {
                            caractereActuel=matrix[i][j]+48;
                            *tmp = ((char)caractereActuel);
                            tmp++;
                        }
                    }
                    *tmp = '\n';
                    tmp++;
                }
                *tmp=0;
            }
            else
            {
                output = (char *)malloc(21);// /!\ /!\ /!\ Modify this value if you modify the text
                putText("2grid can't be solved", output);
            }
        }
        else if(res==1)
        {
            output = (char *)malloc(21);// /!\ /!\ /!\ Modify this value if you modify the text
            putText("grid can't be solved", output);
        }
        else if(res==0)//bad character in string
        {
            output = (char *)malloc(75);// /!\ /!\ /!\ Modify this value if you modify the text
            putText("invalid grid (bad character format in the string or doesn't respect sudoku rules)", output);
        }
    }
    return output;
}



