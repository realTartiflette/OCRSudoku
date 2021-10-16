#include "neuralNetwork.h"
#include <stdio.h>
#include <math.h>
#include <time.h>

float sigmoid(float x)
{
    return (float)(1 / (1 + exp(-x)));
}

float sigmoidPrime (float x)
{
    return x * (1 - x);
}

void getError (matrix expected, matrix output, matrix *error)
{
    for (int i = 0; i < expected.rows; i++)
    {
        for (int j = 0; j < expected.cols; j++)
        {
            error->mat[i][j] = expected.mat[i][j] - output.mat[i][j];
        }
    }

    error->rows = expected.rows, error->cols = expected.cols;
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
        layer l;
        l.nbOfvariables = prevWeigthSize;
        l.nbOfNeurons = nbOfNeuronsByLayer[i];

        l.weigths.rows = prevWeigthSize, l.weigths.cols = nbOfNeuronsByLayer[i];
        fillMatWithRandom(&(l.weigths));
        
        layers[i] = l;
        prevWeigthSize = nbOfNeuronsByLayer[i];
    }
    
    layer outputLayer;
    outputLayer.nbOfvariables = prevWeigthSize;
    outputLayer.nbOfNeurons = outputSize;

    outputLayer.weigths.rows = prevWeigthSize, outputLayer.weigths.cols = outputSize;
    fillMatWithRandom(&(outputLayer.weigths));
    
    neuralNetwork nn = 
    {
        inputSize,
        nbOfhidenLayers,
        layers,
        outputLayer
    };

    return nn;
}

void forwardPropagation(neuralNetwork *nn, matrix inputs, int nbOfInputElems, matrix *result)
{
    matrix buffer;
    
    matrix resLayer;
    resLayer.rows = nbOfInputElems;
    resLayer.cols = nn->inputSize;
    copyMat(inputs, &resLayer);
    
    for (int i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer l = nn->hiddenLayers[i];
        multMat(resLayer, l.weigths, &buffer);
        
        applyFunc(buffer, sigmoid, &resLayer);

        copyMat(resLayer, &(l.resultLayer));
    }

    layer output = nn->outputLayer;

    multMat(resLayer, output.weigths, &buffer);

    applyFunc(buffer, sigmoid, result);
}

