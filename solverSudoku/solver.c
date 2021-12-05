#include <stdio.h>
#include <string.h>
#include "solverSudoku.h"

int main(int argc, char** argv)
{
    FILE* fichier = NULL;
    if (argc == 2)
    {
        fichier = fopen(argv[1], "r");
        char text[130];


        char caractereActuel = ' ';

        if (fichier != NULL)
        {
            int i=0;
            int j=0;
            do
            {
                caractereActuel = fgetc(fichier); 
                text[i]=caractereActuel;
                i++;

            } while (caractereActuel != EOF && i<130); 
            char *solvedSudoku = solve(text);
            FILE* newfichier= fopen(strcat(argv[1],".result"),"w");
            do
            {
                caractereActuel = solvedSudoku[j];
                printf("%c",caractereActuel);
                fputc(((char)caractereActuel),newfichier);
                j++;
            } while (caractereActuel!=EOF && j<130);
    
            fclose(fichier);
        }
        return 0;
    }
    else
    {
        printf("Not enough arguments !!");
        return 1;
    }

}

