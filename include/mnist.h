#ifndef MNIST_H
#define MNIST_H

#include "dataset.h"
#include "network.h"

dataset_s *load_mnist(int number_of_samples, const char *path);
void mnist_draw(neural_network_s *network);
neural_network_s *train_mnist(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes);

#endif
