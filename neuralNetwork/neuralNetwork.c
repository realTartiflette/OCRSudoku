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

void getError (matrix *expected, matrix *output, matrix *error)
{
    for (int i = 0; i < expected->rows; i++)
    {
        for (int j = 0; j < expected->cols; j++)
        {
            error->mat[i][j] = expected->mat[i][j] - output->mat[i][j];
        }
    }

    error->rows = expected->rows, error->cols = expected->cols;
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
        for (int j = 0; j < nn.hiddenLayers[i].weigths.cols; j++)
        {
            printf("o ");
        }
        printf("\n\n");
        
    }

    printf("    ");
    for (int i = 0; i < nn.outputLayer.weigths.cols; i++)
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

        l.weigths.rows = prevWeigthSize, l.weigths.cols = nbOfNeuronsByLayer[i];
        fillMatWithRandom(&(l.weigths));
        
        layers[i] = l;
        prevWeigthSize = nbOfNeuronsByLayer[i];
    }
    
    layer outputLayer;

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

void forwardPropagation(neuralNetwork *nn, matrix inputs, matrix *result)
{
    matrix buffer;
    
    matrix resLayer;
    resLayer.rows = inputs.rows;
    resLayer.cols = nn->inputSize;
    copyMat(inputs, &resLayer);
    
    for (int i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        multMat(resLayer, l->weigths, &buffer);
        
        applyFunc(buffer, sigmoid, &resLayer);

        copyMat(resLayer, &(l->resultLayer));
    }
    
    layer output = nn->outputLayer;

    multMat(resLayer, output.weigths, &buffer);

    applyFunc(buffer, sigmoid, result);
}

void backwardPropagation(neuralNetwork *nn, matrix inputs, matrix expectedResults, matrix results)
{
    
    matrix buffer;

    matrix error;
    matrix outputDelta;

    // COMPUTE ALL DELTAS
    
    //output Layer delta
    layer *output = &(nn->outputLayer);
    getError(&expectedResults, &results, &error);
    applyFunc(results, sigmoidPrime, &buffer);
    
    for (int i = 0; i < results.rows; i++)
    {
        for (int j = 0; j < results.cols; j++)
        {
            outputDelta.mat[i][j] = buffer.mat[i][j] * error.mat[i][j];
        }
    }
    outputDelta.rows = results.rows, outputDelta.cols = results.cols;
    
    //hidden layers delta
    matrix prevDelta;
    copyMat(outputDelta, &prevDelta);
    
    matrix prevWeights;
    copyMat(output->weigths, &prevWeights);
    
    matrix hiddenDeltas[MAX_LAYER];

    for (int i = nn->nbOfHiddenLayers - 1; i >= 0; i--)
    {
        //compute error
        layer *l = &(nn->hiddenLayers[i]);
        matrix layerError;
        transMat(prevWeights, &buffer);
        multMat(prevDelta, buffer, &layerError);

        //compute delta
        applyFunc(l->resultLayer, sigmoidPrime, &buffer);
        

        for (int j = 0; j < layerError.rows; j++)
        {
            for (int k = 0; k < layerError.cols; k++)
            {
                hiddenDeltas[i].mat[j][k] = layerError.mat[j][k] * buffer.mat[j][k];
            }  
        }
        hiddenDeltas[i].rows = layerError.rows, hiddenDeltas[i].cols = layerError.cols;
        copyMat(hiddenDeltas[i], &prevDelta);
        copyMat(l->weigths, &prevWeights);
        
    }
    
    // UPSATE WEIGHTS

    // hidden layers
    matrix prevResults;
    copyMat(inputs, &prevResults);

    for (int i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        matrix trans;
        transMat(prevResults, &trans);
        multMat(trans, hiddenDeltas[i], &buffer);
        addMat(l->weigths, buffer, &(l->weigths));

        copyMat(l->resultLayer, &prevResults);
    }
    
    // output Layer

    matrix trans;
    transMat(prevResults, &trans);
    multMat(trans, outputDelta, &buffer);
    addMat(output->weigths, buffer, &(output->weigths));
}

void trainNetwork(neuralNetwork *nn, matrix inputs, matrix expectedResults, long nbOIter)
{
    for (long i = 0; i < nbOIter; i++)
    {
        matrix res;
        forwardPropagation(nn, inputs, &res);
        backwardPropagation(nn, inputs, expectedResults, res);
        printMat(res);
        printf("\n");
    }
}