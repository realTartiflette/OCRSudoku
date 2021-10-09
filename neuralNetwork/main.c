#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"

int main()
{
	int n[MAX_LAYER] = {2};
	neuralNetwork nn = createNeuralNetwork(2,1,n,1);

	matrix inputs = 
	{
		{1,1},
		{0,0},
		{1,0},
		{0,1}
	};

	matrix res;
	forwardPropagation(&nn, inputs, 4, res);
	printMat(res, 4, nn.outputLayer.nbOfNeurons);
	return 0;
}