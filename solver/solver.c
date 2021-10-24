#include <stdio.h>
#include <string.h>




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
            //cell is unassigned
            if(matrix[i][j] == 0)
            {
                //changing the values of row and col
                *row = i;
                *col = j;
                //there is one or more unassigned cells
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
    //checking in row
    for(i=0;i<9;i++)
    {
        //there is a cell with same value
        if(matrix[r][i] == n)
            return 0;
    }
    //checking column
    for(i=0;i<9;i++)
    {
        //there is a cell with the value equal to i
        if(matrix[i][c] == n)
            return 0;
    }
    //checking sub matrix
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
    //if all cells are assigned then the sudoku is already solved
    //pass by reference because number_unassigned will change the values of row and col
    if(number_unassigned(&row, &col) == 0)
        return 1;
    int i;
    //number between 1 to 9
    for(i=1;i<=9;i++)
    {
        //if we can assign i to the cell or not
        //the cell is matrix[row][col]
        if(is_safe(i, row, col))
        {
            matrix[row][col] = i;
            //backtracking
            if(solve_sudoku())
                return 1;
            //if we can't proceed with this solution
            //reassign the cell
            matrix[row][col]=0;
        }
    }
    return 0;
}




int main(int argc, char* argv[])
{
    FILE* fichier = NULL;
    if (argc == 2)
    {
        fichier = fopen(argv[1], "r");

        char caractereActuel = ' ';

        if (fichier != NULL)
        {
            int i=0;
            int j=0;
            // Boucle de lecture des caractères un à un
            do
            {
                caractereActuel = fgetc(fichier); // On lit le caractère
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
                //printf("%c", caractereActuel); // On l'affiche



            } while (caractereActuel != EOF && i!=9); // On continue tant que fgetc n'a pas retourné EOF (fin de fichier)
    
            fclose(fichier);
        }



        if (solve_sudoku(matrix))
        {
            FILE* newfichier= fopen(strcat(argv[1],".result"),"w");
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


