#include <stdio.h>
#include <stdlib.h>
#include "initNetwork.h"
int main()
{
	
	size_t n[MAX_LAYER] = {1000, 250};
	matrix *inputs;
	matrix *expectedResults;
	char path[100] = "training_set/";

	neuralNetwork *nn = initNetwork(path, 2, n, &inputs, &expectedResults);
	for (size_t i = 0; i < 5; i++)
	{
		printf("iter : %ld\n", i);
		trainNetwork(nn, inputs, expectedResults, 100);
		saveNetwork(nn, "number_detection");
	}
	
	freeMat(inputs);
	freeMat(expectedResults);
	freeNetwork(nn);
	return 0;
}