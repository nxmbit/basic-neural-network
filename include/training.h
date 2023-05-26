#ifndef TRAINING_H
#define TRAINING_H

#include "network.h"
#include "dataset.h"

void feed_forward(neural_network_s *network);
void backpropagation(neural_network_s *network);
double cost(neural_network_s *network);
double success_rate(neural_network_s *network, int sample_index, dataset_s *dataset);
void network_train(neural_network_s *network, dataset_s *dataset, int epochs, double learning_rate);
void apply_gradients(neural_network_s *network, double learning_rate);
void set_expected_output_neurons(neural_network_s *network, dataset_s *dataset, int sample_index);
void result(neural_network_s *network);
void stochastic_network_train(neural_network_s *network, dataset_s *dataset, int epochs, double learning_rate, int batch_size);

#endif