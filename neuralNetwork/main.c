#include <stdio.h>
#include <stdlib.h>
#include "neuralNetwork.h"
int main()
{
	int n[MAX_LAYER] = {6, 4, 2};
	neuralNetwork nn = createNeuralNetwork(5,3,n,1);
	matrix inputs = 
	{
		14, //rows
		5, //cols
		{
			{1,1,1,1,1},
			{0,0,0,0,0},

			{1,0,0,0,0},
			{0,0,0,1,0},
			{0,0,1,0,0},

			{1,0,1,0,0},
			{1,0,0,0,1},
			{0,1,0,1,0},

			{0,1,1,0,1},
			{0,1,0,1,1},
			{1,0,1,0,1},

			{1,0,1,1,1},
			{1,1,1,1,0},
			{1,1,0,1,1}
		}
	};

	matrix expectedResuls = {
		14, //rows
		1, //cols
		{
			{0},
			{0},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
			{1},
		}
	};

	trainNetwork(&nn, inputs, expectedResuls, 3000);

	matrix res;

	matrix test = 
	{
		8, //rows
		5, //cols
		{
			{0,1,0,0,0},
			{0,0,0,0,1},

			{1,1,0,0,0},
			{1,0,0,1,0},

			{1,1,0,0,1},
			{1,0,0,1,1},

			{1,1,1,0,1},
			{0,1,1,1,1}
		}
	};

	forwardPropagation(&nn, test, &res);
	printMat(test);
	printMat(res);
	return 0;
}