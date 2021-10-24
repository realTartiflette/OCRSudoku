#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	int n[MAX_LAYER] = {3};
	neuralNetwork nn = createNeuralNetwork(2,1,n,1);
	matrix inputs = 
	{
		4,//rows
		2,//cols
		{
			{0,0},
			{1,1},
			{0,1},
			{1,0}
		}
	};

	matrix expectedResuls = {
		4, //rows
		1, //cols
		{
			{0},
			{0},
			{1},
			{1},
		}
	};

	trainNetwork(&nn, inputs, expectedResuls, 5000);

	matrix res;

	forwardPropagation(&nn, inputs, &res);
	printMat(inputs);
	printMat(res);
	return 0;
}