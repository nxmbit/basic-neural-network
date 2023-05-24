#ifndef NETWORK_H
#define NETWORK_H

#include "matrixf_s.h"

typedef struct layer_s {
    int layer_size;
    int next_layer_size;
    matrixf_s *neurons;
    matrixf_s *weights;
    matrixf_s *biases;
    matrixf_s *weights_gradient;
    matrixf_s *biases_gradient;
    matrixf_s *neurons_delta;
} layer_s;

typedef struct neural_network_s {
    layer_s **layers;
    matrixf_s *expected_output_neurons;
    int layers_count;
    int *layers_sizes;
} neural_network_s;

layer_s *create_layer(int layer_size, int next_layer_size);
void initialize_weights(neural_network_s *network);
neural_network_s *create_neural_network(int layers_count, int *layers_sizes);
void free_neural_network(neural_network_s *network);
void initialize_biases(neural_network_s *network);
void initialize_gradients(neural_network_s *network);
double gaussian_noise_generator(double mean, double std_deviation);
void save_model(neural_network_s *network, const char *path);
neural_network_s *load_model(const char *path);

#endif