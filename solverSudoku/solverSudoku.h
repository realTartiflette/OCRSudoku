#ifndef SOLVERSUDOKU_H
#define SOLVERSUDOKU_H

void print_sudoku();
int number_unassigned(int *row, int *col);
int is_safe(int n, int r, int c);
int solve_sudoku();
int solve(char *sudoku);

#endif