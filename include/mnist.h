#ifndef MNIST_H
#define MNIST_H

#include "dataset.h"

dataset_s *load_mnist(int number_of_samples, const char *path);
void mnist_draw(neural_network_s *network);

#endif
