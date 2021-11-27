#include <stdio.h>
#include <string.h>
#include "solverSudoku.h"

int main(int argc, char** argv)
{
    if (argc == 2)
    {
        return solve(argv[1]);
    }
    return 1;
}
