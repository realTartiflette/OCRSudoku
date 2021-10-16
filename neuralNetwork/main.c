#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	int n[MAX_LAYER] = {2};
	neuralNetwork nn = createNeuralNetwork(2,1,n,1);
	matrix inputs = 
	{
		4, //rows
		2, //cols
		{
			{1,1},
			{0,0},
			{1,0},
			{0,1}
		}
	};

	matrix expectedResuls = {
		4, //rows
		1, //cols
		{
			{0},
			{1},
			{1},
			{0}
		}
	};

	for (long i = 0; i < 100000; i++)
	{
		matrix res;
		forwardPropagation(&nn, inputs, 4, &res);
		backwardPropagation(&nn, inputs, expectedResuls, res);
	}

	matrix res;
	forwardPropagation(&nn, inputs, 4, &res);
	printMat(res);
	return 0;
}