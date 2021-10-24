#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "matrix.h"

#define MAX_LAYER 10
#define MAX_NEURON 10

#define LEARNING_RATE 0.5

typedef struct layer layer;
struct layer
{
    matrix weigths;
    //matrix bias;
    matrix resultLayer;
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

void forwardPropagation(neuralNetwork *nn, matrix inputs, matrix *result);

void backwardPropagation(neuralNetwork *nn, matrix inputs, matrix expectedResults, matrix results);

void trainNetwork(neuralNetwork *nn, matrix inputs, matrix expectedResults, long nbOIter);
#endif