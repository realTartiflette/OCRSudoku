#include "neuralNetwork.h"
#include <stdio.h>

float getFloatRandom()
{
    return (float)rand()/(float)(RAND_MAX);
}

void printNetwork(neuralNetwork nn)
{
    for (int i = 0; i < nn.inputSize; i++)
    {
        printf("o ");
    }
    printf("\n\n");

    for (int i = 0; i < nn.nbOfHiddenLayers; i++)
    {
        for (int j = 0; j < nn.hiddenLayers[i].nbOfNeurons; j++)
        {
            printf("o ");
        }
        printf("\n\n");
        
    }

    for (int i = 0; i < nn.outputLayer.nbOfNeurons; i++)
    {
        printf("o ");
    }
    
    
}

neuralNetwork createNeuralNetwork(int inputSize, int nbOfhidenLayers, int nbOfNeuronsByLayer[MAX_NEURON], int outputSize)
{
    layer layers[MAX_LAYER];

    //srand((unsigned int) time(NULL));

    int prevWeigthSize = inputSize;
    for (int i = 0; i < nbOfhidenLayers; i++)
    {
        neuron layerNeuron[MAX_NEURON];
        for (int j = 0; j < nbOfNeuronsByLayer[i]; j++)
        {
            neuron n;
            for (int k = 0; k < prevWeigthSize; k++)
            {
                n.weights[k][0] = getFloatRandom();
            }
            /*printMat(n.weights, prevWeigthSize, 1);
            printf("\n");*/
            layerNeuron[j] = n;
        }

        layer l =
        {
            nbOfNeuronsByLayer[i],
            layerNeuron
        };
        
        layers[i] = l;
        prevWeigthSize = nbOfNeuronsByLayer[i];
    }

    
    neuron outputNeurons[MAX_NEURON];

    for (int i = 0; i < outputSize; i++)
    {
        neuron n;
        for (int j = 0; j < prevWeigthSize; j++)
        {
            n.weights[j][0] = getFloatRandom();
        }
        outputNeurons[i] = n;
    }

    
    layer outputLayer = {
        outputSize,
        outputNeurons
    };
    
    
    neuralNetwork nn = 
    {
        inputSize,
        nbOfhidenLayers,
        layers,
        outputLayer
    };

    return nn;
}