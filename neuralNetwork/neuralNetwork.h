#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"

#define MAX_LAYER 10
#define MAX_NEURON 10

#define LEARNING_RATE 0.01

typedef struct layer layer;
struct layer
{
    matrix *weigths;
    float *bias;
    matrix *resultLayer;
};

typedef struct neuralNetwork neuralNetwork;

struct neuralNetwork
{
    size_t inputSize;
    size_t nbOfHiddenLayers;
    struct layer *hiddenLayers;
    struct layer *outputLayer;
};

void printNetwork(neuralNetwork *nn);

neuralNetwork *createNeuralNetwork(size_t inputSize, size_t nbOfhidenLayers, size_t nbOfNeuronsByLayer[MAX_NEURON], size_t outputSize);

void freeNetwork(neuralNetwork *nn);

void forwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *result);

void backwardPropagation(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, matrix *results, matrix *buffer);

void trainNetwork(neuralNetwork *nn, matrix *inputs, matrix *expectedResults, size_t nbOIter);
#endif