#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	
	size_t n[MAX_LAYER] = {3};
	neuralNetwork *nn = createNeuralNetwork(2,1,n,2); //createNeuralNetwork(10000,1,n,10);
	

	matrix *inputs = matAlloc(4,2);
	inputs->mat[0] = 0;
	inputs->mat[1] = 0;
	inputs->mat[2] = 1;
	inputs->mat[3] = 1;
	inputs->mat[4] = 0;
	inputs->mat[5] = 1;
	inputs->mat[6] = 1;
	inputs->mat[7] = 0;

	/*{
		4,//rows
		2,//cols
		{
			{0,0},
			{1,1},
			{0,1},
			{1,0}
		}
	};*/

	matrix *expectedResuls = matAlloc(4,2);
	
	expectedResuls->mat[0] = 0;
	expectedResuls->mat[1] = 1;
	expectedResuls->mat[2] = 0;
	expectedResuls->mat[3] = 1;
	expectedResuls->mat[4] = 1;
	expectedResuls->mat[5] = 0;
	expectedResuls->mat[6] = 1;
	expectedResuls->mat[7] = 0;
	/*{
		4, //rows
		2, //cols
		{
			{0,1},
			{0,1},
			{1,0},
			{1,0},
		}
	};*/
	

	trainNetwork(nn, inputs, expectedResuls, 200000);
	saveNetwork(nn, "test");
	neuralNetwork *nn2 = loadNetwork("test");

	matrix *res = matAlloc(4,2);
	matrix *res2 = matAlloc(4,2);
	forwardPropagation(nn, inputs, res);
	forwardPropagation(nn2, inputs, res2);
	printMat(inputs);
	printf("\n");
	printMat(res);
	printf("\n");
	printMat(res2);


	freeMat(res);
	freeMat(res2);
	freeNetwork(nn);
	freeNetwork(nn2);
	return 0;
}