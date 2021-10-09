#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"

#define MAX_LAYER 10
#define MAX_NEURON 10

typedef struct neuron neuron;
struct neuron
{
    matrix weights;
};

typedef struct layer layer;
struct layer
{
    int nbOfNeurons;
    struct neuron *neurons;
};

typedef struct neuralNetwork neuralNetwork;
struct neuralNetwork
{
    int inputSize;
    int nbOfHiddenLayers;
    struct layer *hiddenLayers;
    struct layer outputLayer;
};

void printNetwork(neuralNetwork nn);

neuralNetwork createNeuralNetwork(int inputSize, int nbOfhidenLayers, int nbOfNeuronsByLayer[MAX_NEURON], int outputSize);

void forwardPropagation(neuralNetwork *nn, matrix inputs, int nbOfInputElems, matrix result);
#endif