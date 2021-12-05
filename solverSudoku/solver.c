#include <stdio.h>
#include <string.h>
#include "solverSudoku.h"

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        FILE* fichier = NULL;
        fichier = fopen(argv[1], "r");
        char text[100];
        if (fichier != NULL)
        {
            fgets(text, 100, fichier);
        }
        char *solvedSudoku = solve(text);
        return 0;
    }
    printf("Not enough arguments !!");
    return 1;
}
