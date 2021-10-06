#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

float test(float value)
{
	return value * 2;
}

int main()
{
	matrix A = 
	{
		{1,2,3},
		{4,5,6},
		{7,8,9},
	};
	
	matrix res;
	applyFunc(A, 3, 3, test, res);
	printMat(res, 3, 3);
}