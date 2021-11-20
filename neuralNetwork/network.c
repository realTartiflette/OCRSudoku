#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	size_t n[MAX_LAYER] = {3};
	neuralNetwork *nn = createNeuralNetwork(10000,1,n,10);

	matrix *inputs = matAlloc(100,10000);
	/*inputs->mat[0] = 0;
	inputs->mat[1] = 0;
	inputs->mat[2] = 1;
	inputs->mat[3] = 1;
	inputs->mat[4] = 0;
	inputs->mat[5] = 1;
	inputs->mat[6] = 1;
	inputs->mat[7] = 0;*/

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

	matrix *expectedResuls = matAlloc(100,10);
	
	/*expectedResuls->mat[0] = 0;
	expectedResuls->mat[1] = 1;
	expectedResuls->mat[2] = 0;
	expectedResuls->mat[3] = 1;
	expectedResuls->mat[4] = 1;
	expectedResuls->mat[5] = 0;
	expectedResuls->mat[6] = 1;
	expectedResuls->mat[7] = 0;*/
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
	

	trainNetwork(nn, inputs, expectedResuls, 1000);

	matrix *res = matAlloc(100,10);
	forwardPropagation(nn, inputs, res);
	//printMat(inputs);
	//printf("\n");
	//printMat(res);

	freeMat(res);
	freeNetwork(nn);
	return 0;
}