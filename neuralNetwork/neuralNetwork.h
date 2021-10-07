#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"

#define MAX_LAYER 10
#define MAX_NEURONE 10

typedef struct neuralNetwork neuralNetwork;
typedef struct layer layer;
typedef struct neuron neuron;

struct neuron
{
    matrix weights;
};

struct layer
{
    int nbOfNeurons;
    struct neuron neurons[MAX_NEURONE];
};

struct neuralNetwork
{
    int inputSize;
    int nbOfLayers;
    struct layer layers[MAX_LAYER];
    int outputSize;
};

neuralNetwork createNeuralNetwork(int imputSize, int nbOfhidenLayers, int *nbOfNeuronsByLayer, int outputSize);

#endif