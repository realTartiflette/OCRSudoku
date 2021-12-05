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
    size_t rows = expected->rows, cols = expected->cols;
    for (size_t i = 0; i < expected->rows; i++)
    {
        for (size_t j = 0; j < expected->cols; j++)
        {
            error->mat[i * cols + j] = expected->mat[i * cols + j] - output->mat[i * cols + j];
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

size_t max(size_t a, size_t b)
{
    size_t res = b;
    if(a > b) res = a;
    return res;
}

neuralNetwork *createNeuralNetwork(size_t inputSize, size_t nbOfhidenLayers, size_t nbOfNeuronsByLayer[MAX_NEURON], size_t outputSize)
{
    layer *layers = calloc(nbOfhidenLayers, sizeof(layer));
    srand((unsigned int) time(NULL));

    size_t prevWeigthSize = inputSize;
    for (size_t i = 0; i < nbOfhidenLayers; i++)
    {
        layer l;
        l.weigths = matAlloc(prevWeigthSize, nbOfNeuronsByLayer[i]);
        l.bias = calloc(nbOfNeuronsByLayer[i], sizeof(float));
        l.resultLayer = NULL;

        fillMatWithRandom(l.weigths);

        for (size_t j = 0; j < nbOfNeuronsByLayer[i]; j++)
        {
            l.bias[j] = 2*((float)rand()/(float)(RAND_MAX)) - 1;
        }
        
        layers[i] = l;
        prevWeigthSize = nbOfNeuronsByLayer[i];
    }
    
    layer *outputLayer = malloc(sizeof(layer));

    outputLayer->weigths = matAlloc(prevWeigthSize, outputSize);
    outputLayer->resultLayer = NULL;
    outputLayer->bias = calloc(outputSize, sizeof(float));

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

void RLalloc(neuralNetwork *nn, size_t nbOfInput)
{
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        l->resultLayer = matAlloc(nbOfInput, l->weigths->cols);
    }
    
    layer *output = nn->outputLayer;
    output->resultLayer = matAlloc(nbOfInput, output->weigths->cols);
}

void freeRL(neuralNetwork *nn)
{
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        freeMat(nn->hiddenLayers[i].resultLayer);
    }
    freeMat(nn->outputLayer->resultLayer);
}

void freeNetwork(neuralNetwork *nn)
{
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        free(nn->hiddenLayers[i].weigths);
        free(nn->hiddenLayers[i].bias);
    }
    
    free(nn->hiddenLayers);
    free(nn->outputLayer->bias);
    free(nn->outputLayer);
    free(nn);
}

void _forwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *buffer, matrix *result)
{   
    matrix *resLayer = inputs;
    
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        multMat(resLayer, l->weigths, buffer);
        for (size_t k = 0; k < buffer->cols; k++)
        {
            for (size_t j = 0; j < buffer->rows; j++)
            {
                buffer->mat[j * buffer->cols + k] += l->bias[k];
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
            buffer->mat[i * buffer->cols + j] += output->bias[j];
        }
    }
    
    applyFunc(buffer, sigmoid, result);
}

void forwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *result)
{
    size_t maxSize = max(inputs->rows, inputs->cols);
    matrix *buffer = matAlloc(maxSize, maxSize);
    RLalloc(nn, inputs->rows);
    
    _forwardPropagation(nn, inputs, buffer, result);

    freeRL(nn);
    freeMat(buffer);
}

void backwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, matrix *results, matrix *buffer)
{
    matrix *error = matAlloc(results->rows, results->cols);
    matrix *outputDelta = matAlloc(results->rows, results->cols);

    // COMPUTE ALL DELTAS
    
    //output Layer delta
    layer *output = nn->outputLayer;
    getError(expectedResults, results, error);
    applyFunc(results, sigmoidPrime, buffer);
    
    for (size_t i = 0; i < results->rows; i++)
    {
        for (size_t j = 0; j < results->cols; j++)
        {
            outputDelta->mat[i * error->cols + j] = buffer->mat[i * error->cols + j] * error->mat[i * error->cols + j];
        }
    }

    freeMat(error);
    
    //hidden layers delta
    matrix *prevDelta = outputDelta;
    matrix *prevWeights = output->weigths;
    matrix **hiddenDeltas = calloc(nn->nbOfHiddenLayers, sizeof(matrix *));
    

    for (size_t i = nn->nbOfHiddenLayers; i > 0; i--)
    {
        //compute error
        layer *l = &(nn->hiddenLayers[i - 1]);
        
        transMat(prevWeights, buffer);
        matrix *layerError = matAlloc(prevDelta->rows, buffer->cols);
        multMat(prevDelta, buffer, layerError);

        //compute delta
        applyFunc(l->resultLayer, sigmoidPrime, buffer);
        hiddenDeltas[i - 1] = matAlloc(buffer->rows, buffer->cols);

        for (size_t j = 0; j < layerError->rows; j++)
        {
            for (size_t k = 0; k < layerError->cols; k++)
            {
                hiddenDeltas[i - 1]->mat[j* buffer->cols + k] = layerError->mat[j* buffer->cols + k] * buffer->mat[j* buffer->cols + k];
            }  
        }
        hiddenDeltas[i - 1]->rows = layerError->rows, hiddenDeltas[i - 1]->cols = layerError->cols;
        prevDelta = hiddenDeltas[i - 1];
        prevWeights = l->weigths;
        
        freeMat(layerError);
    }
    
    
    // UPSATE WEIGHTS AND BIAS

    // hidden layers
    matrix *prevResults = inputs;
    
    
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        //update weights
        layer *l = &(nn->hiddenLayers[i]);
        matrix *trans = matAlloc(prevResults->cols, prevResults->rows);
        
        transMat(prevResults, trans);
        multMat(trans, hiddenDeltas[i], buffer);
        applyFunc(buffer, learningRate, buffer);
        addMat(l->weigths, buffer, l->weigths);

        //update bias
        for (size_t k = 0; k < hiddenDeltas[i]->cols; k++)
        {
            float sum = 0;
            for (size_t j = 0; j < hiddenDeltas[i]->rows; j++)
            {
                sum += hiddenDeltas[i]->mat[j * hiddenDeltas[i]->cols + k];
            }
            l->bias[k] += LEARNING_RATE * sum;
        }
        
        prevResults = l->resultLayer;
        freeMat(hiddenDeltas[i]);
        freeMat(trans);
    }
    
    // output Layer

    //update weights
    matrix *trans = matAlloc(prevResults->cols, prevResults->rows);

    transMat(prevResults, trans);
    multMat(trans, outputDelta, buffer);
    applyFunc(buffer, learningRate, buffer);
    addMat(output->weigths, buffer, output->weigths);
    
    //update bias
    for(size_t j = 0; j < outputDelta->cols; j++)
    {
        float sum = 0;
        for (size_t i = 0; i < outputDelta->rows; i++)
        {
            sum += outputDelta->mat[i * outputDelta->cols + j];
        }
        output->bias[j] += LEARNING_RATE * sum;
    }

    freeMat(outputDelta);
    free(hiddenDeltas);
    freeMat(trans);
}

void trainNetwork(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, size_t nbOIter)
{
    matrix *res = matAlloc(inputs->rows, nn->outputLayer->weigths->cols);
    size_t maxSize = max(inputs->rows, inputs->cols);
    matrix *buffer = matAlloc(maxSize, maxSize);
    RLalloc(nn, inputs->rows);
    
    for (size_t i = 0; i < nbOIter; i++)
    {
        _forwardPropagation(nn, inputs, buffer, res);
        backwardPropagation(nn, inputs, expectedResults, res, buffer);
        if(i%10 == 0) printf("%ld\n", i);
    }
    freeRL(nn);
    freeMat(buffer);
    freeMat(res);
}

void saveNetwork(neuralNetwork *nn)
{
    FILE *f = fopen("network_info", "w");

    //Save all network infos
    fprintf(f, "%ld\n%ld\n", nn->inputSize, nn->nbOfHiddenLayers);
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        fprintf(f, "%ld\n", l->weigths->cols);
    }
    layer *out = nn->outputLayer;
    fprintf(f, "%ld\n", out->weigths->cols);

    fclose(f);
    

    f = fopen("weights_and_biases", "w");
    for (size_t i = 0; i < nn->nbOfHiddenLayers; i++)
    {
        layer *l = &(nn->hiddenLayers[i]);
        //write bias   
        for (size_t j = 0; j < l->weigths->cols; j++)
        {
            fprintf(f, "%.3g\n", l->bias[j]);
        }
        fprintf(f, "\n");
        //write weights
        size_t len = l->weigths->rows * l->weigths->cols;
        for (size_t j = 0; j < len; j++)
        {
            fprintf(f, "%.3g\n", l->weigths->mat[j]);
        }
        fprintf(f, "\n");
    }

    //write biases of outputs
    for (size_t i = 0; i < out->weigths->cols; i++)
    {
        fprintf(f, "%.3g\n", out->bias[i]);
    }
    fprintf(f, "\n");
    
    //write weights of outputs
    size_t len = out->weigths->rows * out->weigths->cols;
    for (size_t i = 0; i < len; i++)
    {
        fprintf(f, "%.3g\n", out->weigths->mat[i]);
    }

    fclose(f);
}

neuralNetwork *loadNetwork()
{
    FILE *f = fopen("network_info", "r");
    size_t inputSize;
    fscanf(f, "%ld\n", &inputSize);

    size_t nbOfHiddenLayers;
    fscanf(f, "%ld\n", &nbOfHiddenLayers);
    //printf("%ld\n", nbOfHiddenLayers);
    
    FILE *f1 = fopen("weights_and_biases", "r");

    layer *layers = calloc(nbOfHiddenLayers, sizeof(layer));
    size_t prevWeigthSize = inputSize;

    for (size_t i = 0; i < nbOfHiddenLayers; i++)
    {
        layer l;
        size_t nbOfNeurons;
        fscanf(f, "%ld\n", &nbOfNeurons);
        //printf("%ld\n", nbOfNeurons);

        //read biases
        l.bias = calloc(nbOfNeurons, sizeof(float));
        for (size_t j = 0; j < nbOfNeurons; j++)
        {
            fscanf(f1, "%f\n", &(l.bias[j]));
        }
        fscanf(f1, "\n");

        //read weights
        l.weigths = matAlloc(prevWeigthSize, nbOfNeurons);
        size_t len = prevWeigthSize * nbOfNeurons;
        for (size_t j = 0; j < len; j++)
        {
            fscanf(f1, "%f\n", &(l.weigths->mat[j]));
        }
        fscanf(f1, "\n");
        
        l.resultLayer = NULL;
        layers[i] = l;
        prevWeigthSize = nbOfNeurons;
    }

    layer *outputLayer = malloc(sizeof(layer));
    size_t outputSize;
    fscanf(f, "%ld\n", &outputSize);
    //printf("%ld\n", outputSize);

    outputLayer->bias = calloc(outputSize, sizeof(float));
    for (size_t i = 0; i < outputSize; i++)
    {
        fscanf(f1, "%f\n", &(outputLayer->bias[i]));
    }
    fscanf(f1, "\n");

    outputLayer->weigths = matAlloc(prevWeigthSize, outputSize);
    size_t len = prevWeigthSize * outputSize;
    for (size_t i = 0; i < len; i++)
    {
        fscanf(f1, "%f\n", &(outputLayer->weigths->mat[i]));
    }
    
    outputLayer->resultLayer = NULL;

    neuralNetwork *nn = malloc(sizeof(neuralNetwork));
    nn->inputSize = inputSize;
    nn->nbOfHiddenLayers = nbOfHiddenLayers;
    nn->hiddenLayers = layers;
    nn->outputLayer = outputLayer;
    
    fclose(f);
    fclose(f1);
    return nn;
}