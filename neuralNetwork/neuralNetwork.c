#include "neuralNetwork.h"
#include <stdio.h>

float getFloatRandom()
{
    srand((unsigned int) time(NULL));
    return (float)rand()/(float)(RAND_MAX);
}

neuralNetwork createNeuralNetwork(int imputSize, int nbOfhidenLayers, int *nbOfNeuronsByLayer, int outputSize)
{
    layer layers[MAX_LAYER];

    srand((unsigned int) time(NULL));
    int prevWeigthSize = imputSize;
    for (int i = 0; i < nbOfhidenLayers; i++)
    {
        for (int j = 0; j < nbOfNeuronsByLayer[i]; j++)
        {
            matrix w;
            for (int k = 0; k < prevWeigthSize; k++)
            {
                w[k][0] = getFloatRandom();
            }
            printMat(w, prevWeigthSize, 1);
            printf("\n");
        }
        
    }
    
}