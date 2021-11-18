#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	size_t n[MAX_LAYER] = {3};
	neuralNetwork *nn = createNeuralNetwork(2,1,n,2);
	

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
		2, //cols
		{
			{0,1},
			{0,1},
			{1,0},
			{1,0},
		}
	};

	trainNetwork(nn, &inputs, &expectedResuls, 200000);

	matrix res;

	forwardPropagation(nn, &inputs, &res);
	printMat(&inputs);
	printf("\n");
	printMat(&res);

	freeNetwork(nn);
	return 0;
}