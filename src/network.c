#include "matrixf_s.h"
#include "network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

neural_network_s *create_neural_network(int layers_count, int *layers_sizes) {
    neural_network_s *network = malloc(sizeof(neural_network_s));
    network->layers = malloc(layers_count * sizeof(layer_s));
    network->layers_count = layers_count;
    network->layers_sizes = layers_sizes;
    network->expected_output_neurons = create_matrixf(layers_sizes[layers_count - 1], 1);
    for (int i = 0; i < layers_count; i++) {
        if (i == layers_count - 1) {
            network->layers[i] = create_layer(layers_sizes[i], 0);
        } else {
            network->layers[i] = create_layer(layers_sizes[i], layers_sizes[i + 1]);
        }
    }
    initialize_weights(network);
    initialize_biases(network);
    initialize_gradients(network);
    return network;
}

layer_s *create_layer(int layer_size, int next_layer_size) {
    layer_s *layer = malloc(sizeof(layer_s));
    layer->layer_size = layer_size;
    layer->next_layer_size = next_layer_size;
    layer->neurons = create_matrixf(layer_size, 1);
    if (next_layer_size != 0) {
        layer->weights = create_matrixf(next_layer_size, layer_size);
        layer->biases = create_matrixf(next_layer_size, 1);
        layer->weights_gradient = create_matrixf(next_layer_size, layer_size);
        layer->biases_gradient = create_matrixf(next_layer_size, 1);
        layer->neurons_delta = create_matrixf(next_layer_size, 1);
        layer->weighed_sums = create_matrixf(next_layer_size, 1);
    }

    return layer;
}

void free_neural_network(neural_network_s *network) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        matrixf_free(network->layers[i]->neurons);
        matrixf_free(network->layers[i]->weights);
        matrixf_free(network->layers[i]->biases);
        matrixf_free(network->layers[i]->weights_gradient);
        matrixf_free(network->layers[i]->biases_gradient);
        free(network->layers[i]);
    }
    matrixf_free(network->layers[network->layers_count - 1]->neurons);
    matrixf_free(network->expected_output_neurons);
    free(network->layers[network->layers_count - 1]);
    free(network->layers);
    free(network);
}

void initialize_weights(neural_network_s *network) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->weights->rows; j++) {
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                network->layers[i]->weights->tab[j][k] = gaussian_noise_generator(0, 1);
            }
        }
    }
}

void initialize_biases(neural_network_s * network) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->biases->rows; j++) {
            network->layers[i]->biases->tab[j][0] = 0;
        }
    }
}

void initialize_gradients(neural_network_s *network) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->biases_gradient->rows; j++) {
            network->layers[i]->biases_gradient->tab[j][0] = 0;
            network->layers[i]->neurons_delta->tab[j][0] = 0;
            for (int k = 0; k < network->layers[i]->weights_gradient->cols; k++) {
                network->layers[i]->weights_gradient->tab[j][k] = 0;
            }
        }
    }
}

double gaussian_noise_generator(double mean, double std_deviation) {
    double u1 = drand48();
    double u2 = drand48();
    //box muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return (std_deviation * z0) + mean;
}

void save_model(neural_network_s *network, const char *path) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        printf("Error while opening file %s\n", path);
        exit(1);
    }
    fprintf(file, "%d\n", network->layers_count);
    for (int i = 0; i < network->layers_count; i++) {
        fprintf(file, "%d\n", network->layers_sizes[i]);
    }
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->weights->rows; j++) {
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                fprintf(file, "%f\n", network->layers[i]->weights->tab[j][k]);
            }
        }
    }
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->biases->rows; j++) {
            fprintf(file, "%f\n", network->layers[i]->biases->tab[j][0]);
        }
    }
    fclose(file);
}

neural_network_s *load_model(const char *path) {
    neural_network_s *network;
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Error while opening file %s\n", path);
        return NULL;
    }

    int layers_count = 0;
    fscanf(file, "%d\n", &layers_count);
    int *layers_sizes = malloc(layers_count * sizeof(int));
    for (int i = 0; i < layers_count; i++) {
        fscanf(file, "%d\n", &layers_sizes[i]);
    }
    network = create_neural_network(layers_count, layers_sizes);

    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->weights->rows; j++) {
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                fscanf(file, "%lf\n", &network->layers[i]->weights->tab[j][k]);
            }
        }
    }
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->biases->rows; j++) {
            fscanf(file, "%lf\n", &network->layers[i]->biases->tab[j][0]);
        }
    }
    fclose(file);
    return network;
}
