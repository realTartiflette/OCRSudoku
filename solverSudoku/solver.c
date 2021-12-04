#include <stdio.h>
#include <string.h>
#include "solverSudoku.h"

int main(int argc, char** argv)
{
    printf("%s\n", solve("... ..4 58.\n... 721 ..3\n4.3 ... ...\n\n21. .67 ..4\n.7. ... 2..\n63. .49 ..1\n\n1.6 ... ...\n... 158 ..6\n... ..6 95."));
    return 0;
}
