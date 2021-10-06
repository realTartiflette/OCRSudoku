#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
	matrix A = 
	{
		{1,2},
		{3,4}
	};

	matrix B = 
	{
		{1,2},
		{3,4}
	};
	
	matrix res;
	multMat(A, 2, 2, B, 2, 2, res);
	printMat(res, 2, 2);
}