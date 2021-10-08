#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"

int main()
{
	int n[MAX_LAYER] = {3,2};
	neuralNetwork nn = createNeuralNetwork(2,2,n,1);
	printNetwork(nn);
	return 0;
}