#ifndef COLOR_CLASSIFICATION_H
#define COLOR_CLASSIFICATION_H

#include "network.h"

#define COLOR_SELECTOR_PATH "../utils/color_classification/select_color.py"
#define COLOR_DATASET_PATH "../datasets/color_classification/colors_cielab.csv"

neural_network_s *train_color_classification(const char *path, int epochs, double learning_rate, int batch_size, int layers_count,  int *layers_sizes);
void select_color(neural_network_s *network);

#endif
