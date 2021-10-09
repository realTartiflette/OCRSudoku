#include "neuralNetwork.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

float getFloatRandom()
{
    return (float)rand()/(float)(RAND_MAX);
}

float sigmoid(float x)
{
    return (float)(1 / (1 + exp(-x)));
}

void printNetwork(neuralNetwork nn)
{
    printf("    ");
    for (int i = 0; i < nn.inputSize; i++)
    {
        printf("o ");
    }
    printf("\n\n");

    for (int i = 0; i < nn.nbOfHiddenLayers; i++)
    {
        printf("    ");
        for (int j = 0; j < nn.hiddenLayers[i].nbOfNeurons; j++)
        {
            printf("o ");
        }
        printf("\n\n");
        
    }

    printf("    ");
    for (int i = 0; i < nn.outputLayer.nbOfNeurons; i++)
    {
        printf("o ");
    }
    
    
}

neuralNetwork createNeuralNetwork(int inputSize, int nbOfhidenLayers, int nbOfNeuronsByLayer[MAX_NEURON], int outputSize)
{
    layer layers[MAX_LAYER];

    srand((unsigned int) time(NULL));

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

void forwardPropagation(neuralNetwork *nn, matrix inputs, int nbOfInputElems, matrix result)
{
    matrix resLayer;
    int resLayerRows = nbOfInputElems;
    int resLayerCols = nn->inputSize;
    copyMat(inputs, resLayerRows, resLayerCols, resLayer);
    
    for (int i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer l = nn->hiddenLayers[i];
        matrix tmp;
        for (int j = 0; j < l.nbOfNeurons; j++)
        {
            matrix resNeuron;
            multMat(resLayer, resLayerRows, resLayerCols, l.neurons[j].weights, resLayerCols, 1, resNeuron);
            
            for (int x = 0; x < resLayerRows; x++)
            {
                tmp[x][j] = resNeuron[x][0];
            }
        }
        applyFunc(tmp, resLayerRows, l.nbOfNeurons, sigmoid, resLayer);
        resLayerCols = l.nbOfNeurons;
    }

    layer output = nn->outputLayer;
    matrix tmp;
    for (int i = 0; i < output.nbOfNeurons; i++)
    {
        matrix resOutput;
        multMat(resLayer, resLayerRows, resLayerCols, output.neurons[i].weights, resLayerCols, 1, resOutput);
        for (int x = 0; x < resLayerRows; x++)
        {
            tmp[x][i] = resOutput[x][0];
        }
    }

    applyFunc(tmp, resLayerRows, output.nbOfNeurons, sigmoid, result);
}