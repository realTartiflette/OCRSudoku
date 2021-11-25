#ifndef INIT_NETWORK_H
#define INIT_NETWORK_H

#include "neuralNetwork.h"

neuralNetwork *initNetwork(char *data_set_path, size_t nbOfHiddenLayers, size_t nbOfNeuronsByLayers[MAX_LAYER], matrix **inputs, matrix **expectedResults);

#endif