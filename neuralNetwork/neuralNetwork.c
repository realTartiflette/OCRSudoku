#include "neuralNetwork.h"
#include <stdio.h>
#include <stdlib.h>
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

float learningRate(float x)
{
    return LEARNING_RATE * x;
}

void getError (matrix *expected, matrix *output, matrix *error)
{
    for (size_t i = 0; i < expected->rows; i++)
    {
        for (size_t j = 0; j < expected->cols; j++)
        {
            error->mat[i][j] = expected->mat[i][j] - output->mat[i][j];
        }
    }

    error->rows = expected->rows, error->cols = expected->cols;
}

void printNetwork(neuralNetwork *nn)
{
    printf("    ");
    for (size_t i = 0; i < nn->inputSize; i++)
    {
        printf("o ");
    }
    printf("\n\n");

    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        printf("    ");
        for (size_t j = 0; j < nn->hiddenLayers[i].weigths->cols; j++)
        {
            printf("o ");
        }
        printf("\n\n");
        
    }

    printf("    ");
    for (size_t i = 0; i < nn->outputLayer->weigths->cols; i++)
    {
        printf("o ");
    }
    
    
}

neuralNetwork *createNeuralNetwork(size_t inputSize, size_t nbOfhidenLayers, size_t nbOfNeuronsByLayer[MAX_NEURON], size_t outputSize)
{
    layer *layers = calloc(nbOfhidenLayers, sizeof(layer));
    srand((unsigned int) time(NULL));

    size_t prevWeigthSize = inputSize;
    for (size_t i = 0; i < nbOfhidenLayers; i++)
    {
        layer l;
        l.weigths = malloc(sizeof(matrix));
        l.bias = calloc(nbOfNeuronsByLayer[i], sizeof(float));
        l.resultLayer = malloc(sizeof(matrix));

        l.weigths->rows = prevWeigthSize;
        l.weigths->cols = nbOfNeuronsByLayer[i];
        fillMatWithRandom(l.weigths);

        for (size_t j = 0; j < nbOfNeuronsByLayer[i]; j++)
        {
            l.bias[j] = 2*((float)rand()/(float)(RAND_MAX)) - 1;
        }
        
        layers[i] = l;
        prevWeigthSize = nbOfNeuronsByLayer[i];
    }
    
    layer *outputLayer = malloc(sizeof(layer));

    outputLayer->weigths = malloc(sizeof(matrix));
    outputLayer->resultLayer = malloc(sizeof(matrix));
    outputLayer->bias = calloc(outputSize, sizeof(float));

    outputLayer->weigths->rows = prevWeigthSize, outputLayer->weigths->cols = outputSize;
    fillMatWithRandom(outputLayer->weigths);
    
    for (size_t i = 0; i < outputSize; i++)
    {
        outputLayer->bias[i] = 2*((float)rand()/(float)(RAND_MAX)) - 1;
    }
    
    neuralNetwork *nn = malloc(sizeof(neuralNetwork));

    nn->inputSize = inputSize;
    nn->nbOfHiddenLayers = nbOfhidenLayers;
    nn->hiddenLayers = layers;
    nn->outputLayer = outputLayer;

    return nn;
}

void freeNetwork(neuralNetwork *nn)
{
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        free(nn->hiddenLayers[i].weigths);
        free(nn->hiddenLayers[i].bias);
        free(nn->hiddenLayers[i].resultLayer);
    }
    
    free(nn->hiddenLayers);
    free(nn->outputLayer->bias);
    free(nn->outputLayer);
    free(nn);
}

void forwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *result)
{
    matrix *buffer = malloc(sizeof(matrix));
    
    matrix *resLayer = inputs; 
    
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        multMat(resLayer, l->weigths, buffer);
        for (size_t k = 0; k < buffer->cols; k++)
        {
            for (size_t j = 0; j < buffer->rows; j++)
            {
                buffer->mat[j][k] += l->bias[k];
            }
            
        }
        
        applyFunc(buffer, sigmoid, l->resultLayer);

        resLayer = l->resultLayer;
    }
    
    layer *output = nn->outputLayer;

    multMat(resLayer, output->weigths, buffer);

    for (size_t j = 0; j < buffer->cols; j++)
    {
        for (size_t i = 0; i < buffer->rows; i++)
        {
            buffer->mat[i][j] += output->bias[j];
        }
    }
    
    applyFunc(buffer, sigmoid, result);
    
    free(buffer);
}

void backwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, matrix *results)
{
    
    matrix *buffer = malloc(sizeof(matrix));

    matrix *error = malloc(sizeof(matrix));
    matrix *outputDelta = malloc(sizeof(matrix));

    // COMPUTE ALL DELTAS
    
    //output Layer delta
    layer *output = nn->outputLayer;
    getError(expectedResults, results, error);
    applyFunc(results, sigmoidPrime, buffer);
    
    for (size_t i = 0; i < results->rows; i++)
    {
        for (size_t j = 0; j < results->cols; j++)
        {
            outputDelta->mat[i][j] = buffer->mat[i][j] * error->mat[i][j];
        }
    }
    outputDelta->rows = results->rows, outputDelta->cols = results->cols;

    free(error);
    
    //hidden layers delta
    matrix *prevDelta = outputDelta;
    matrix *prevWeights = output->weigths;
    matrix *hiddenDeltas = calloc(nn->nbOfHiddenLayers, sizeof(matrix)); //[MAX_LAYER];
    matrix *layerError = malloc(sizeof(matrix));

    for (size_t i = nn->nbOfHiddenLayers; i > 0; i--)
    {
        //compute error
        layer *l = &(nn->hiddenLayers[i - 1]);
        
        transMat(prevWeights, buffer);
        multMat(prevDelta, buffer, layerError);

        //compute delta
        applyFunc(l->resultLayer, sigmoidPrime, buffer);

        for (size_t j = 0; j < layerError->rows; j++)
        {
            for (size_t k = 0; k < layerError->cols; k++)
            {
                hiddenDeltas[i - 1].mat[j][k] = layerError->mat[j][k] * buffer->mat[j][k];
            }  
        }
        hiddenDeltas[i - 1].rows = layerError->rows, hiddenDeltas[i - 1].cols = layerError->cols;
        prevDelta = &hiddenDeltas[i - 1];
        prevWeights = l->weigths;
        
    }

    free(layerError);
    
    // UPSATE WEIGHTS AND BIAS

    // hidden layers
    matrix *prevResults = inputs;
    matrix *trans = malloc(sizeof(matrix));
    
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        //update weights
        layer *l = &(nn->hiddenLayers[i]);
        transMat(prevResults, trans);
        multMat(trans, &hiddenDeltas[i], buffer);
        applyFunc(buffer, learningRate, buffer);
        addMat(l->weigths, buffer, l->weigths);

        //update bias
        for (size_t k = 0; k < hiddenDeltas[i].cols; k++)
        {
            float sum = 0;
            for (size_t j = 0; j < hiddenDeltas[i].rows; j++)
            {
                sum += hiddenDeltas[i].mat[j][k];
            }
            l->bias[k] += LEARNING_RATE * sum;
        }
        
        prevResults = l->resultLayer;
    }
    
    // output Layer
    transMat(prevResults, trans);
    multMat(trans, outputDelta, buffer);
    applyFunc(buffer, learningRate, buffer);
    addMat(output->weigths, buffer, output->weigths);
    free(buffer);
    free(outputDelta);
    free(hiddenDeltas);
    free(trans);
}

void trainNetwork(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, size_t nbOIter)
{
    matrix *res = malloc(sizeof(matrix));
    for (size_t i = 0; i < nbOIter; i++)
    {
        forwardPropagation(nn, inputs, res);
        backwardPropagation(nn, inputs, expectedResults, res);
    }
    free(res);
}