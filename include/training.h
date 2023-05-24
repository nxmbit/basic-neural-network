#ifndef TRAINING_H
#define TRAINING_H

#include "network.h"
#include "dataset.h"

void feed_forward(neural_network_s *network);
void backpropagation(neural_network_s *network);
double cost(neural_network_s *network);
double success_rate(neural_network_s *network);
void network_train(neural_network_s *network, dataset_s *dataset, int epochs, double learning_rate);

#endif