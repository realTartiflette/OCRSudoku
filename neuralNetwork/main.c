#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

int main()
{
	matrix A = 
	{
		{1,2,3},
		{4,5,6},
		{7,8,9},
		{10,11,12}
	};
	
	matrix res;
	transMat(A, 4, 3, res);
	printMat(res, 3, 4);
}