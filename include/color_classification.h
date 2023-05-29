#ifndef COLOR_CLASSIFICATION_H
#define COLOR_CLASSIFICATION_H

#include "network.h"

neural_network_s *train_color_classification(const char *path, int epochs, double learning_rate, int batch_size, int layers_count,  int *layers_sizes);
void select_color(neural_network_s *network);

#endif
