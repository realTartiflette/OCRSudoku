#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
	matrix A = 
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};

	matrix B = 
	{
		{1,2,3},
		{4,5,6},
		{7,8,9}
	};
	
	matrix res;
	addMat(A, 3, 3, B, 3, 3, res);
	printMat(res, 3, 3);
}