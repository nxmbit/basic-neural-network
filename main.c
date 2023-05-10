#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>
#include "include/matrixf_s.h"
#include "include/activations.h"
#include <string.h>

typedef struct layer_s {
    int layer_size;
    int next_layer_size;
    matrixf_s *neurons;
    matrixf_s *weights;
    matrixf_s *biases;
    matrixf_s *weights_cost_gradient;
    matrixf_s *biases_cost_gradient;
} layer_s;

typedef struct neural_network_s {
    layer_s **layers;
    int layers_count;
    int *layers_sizes;
    double cost;
} neural_network_s;

typedef struct dataset_s {
    matrixf_s *inputs;
    matrixf_s *expected_outputs;
    int number_of_samples;
    int input_neurons;
} dataset_s;

layer_s *create_layer(int layer_size, int next_layer_size);
void initialize_weights(neural_network_s *network);
double gaussian_noise_generator(double mean, double std_deviation);
neural_network_s *create_neural_network(int layers_count, int *layers_sizes);
void free_neural_network(neural_network_s *network);
void free_layer(layer_s *layer);
void initialize_biases(neural_network_s *network);
void initialize_gradients(neural_network_s *network);
void feed_forward(neural_network_s *network);
void backpropagation(neural_network_s *network, dataset_s *dataset);

dataset_s *create_dataset(int input_neurons, int number_of_samples) {
    dataset_s *dataset = malloc(sizeof(dataset_s));
    dataset->inputs = create_matrix(number_of_samples, input_neurons);
    dataset->expected_outputs = create_matrix(number_of_samples, 1);
    dataset->number_of_samples = number_of_samples;
    dataset->input_neurons = input_neurons;
    return dataset;
}

dataset_s *load_data_csv(const char *path, int input_neurons, int number_of_samples) {
    dataset_s *dataset = create_dataset(input_neurons, number_of_samples);
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Error while opening file %s\n", path);
        exit(1);
    }
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int i = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        char *token = strtok(line, ",");
        int j = 0;
        while (token != NULL) {
            if (j < input_neurons) {
                dataset->inputs->tab[i][j] = atof(token);
            } else {
                dataset->expected_outputs->tab[i][0] = atof(token);
            }
            token = strtok(NULL, ",");
            j++;
        }
        if (i == 9) {
            break;
        }
        i++;
    }
    fclose(file);
    return dataset;
}

void free_dataset(dataset_s *dataset) {
    matrixf_free(dataset->inputs);
    matrixf_free(dataset->expected_outputs);
    free(dataset);
}


layer_s *create_layer(int layer_size, int next_layer_size) {
    layer_s *layer = malloc(sizeof(layer_s));
    layer->layer_size = layer_size;
    layer->next_layer_size = next_layer_size;
    layer->neurons = create_matrix(layer_size, 1);
    if (next_layer_size != 0) {
        layer->weights = create_matrix(next_layer_size, layer_size);
        layer->biases = create_matrix(next_layer_size, 1);
    }

    layer->weights_cost_gradient = create_matrix(layer_size, 1);
    layer->biases_cost_gradient = create_matrix(layer_size, 1);

    return layer;
}

void free_neural_network(neural_network_s *network) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        matrixf_free(network->layers[i]->neurons);
        matrixf_free(network->layers[i]->weights);
        matrixf_free(network->layers[i]->biases);
        matrixf_free(network->layers[i]->weights_cost_gradient);
        matrixf_free(network->layers[i]->biases_cost_gradient);
        free(network->layers[i]);
    }
    matrixf_free(network->layers[network->layers_count - 1]->neurons);
    free(network->layers[network->layers_count - 1]);
    free(network->layers);
    //free(network->layers_sizes);
    free(network);
}

 neural_network_s *create_neural_network(int layers_count, int *layers_sizes) {
    neural_network_s *network = malloc(sizeof(neural_network_s));
    network->layers = malloc(layers_count * sizeof(layer_s));
    network->layers_count = layers_count;
    network->layers_sizes = layers_sizes;
    for (int i = 0; i < layers_count; i++) {
        if (i == layers_count - 1) {
            network->layers[i] = create_layer(layers_sizes[i], 0);
        } else {
            network->layers[i] = create_layer(layers_sizes[i], layers_sizes[i + 1]);
        }
    }
    network->cost = 0;
    initialize_weights(network);
    initialize_biases(network);
    initialize_gradients(network);
    return network;
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
    for (int i = 0; i < network->layers_count; i++) {
        for (int j = 0; j < network->layers[i]->biases_cost_gradient->rows; j++) {
            network->layers[i]->biases_cost_gradient->tab[j][0] = 0;
            network->layers[i]->weights_cost_gradient->tab[j][0] = 0;
        }
    }
}

double gaussian_noise_generator(double mean, double std_deviation) {
    srand48(time(NULL));
    double u1 = drand48();
    double u2 = drand48();
    //box muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return (std_deviation * z0) + mean;
}


void feed_forward(neural_network_s *network) {
    matrixf_s *multiplication_result;
    for (int i = 0; i < network->layers_count - 1; i++) {
        multiplication_result = matrix_multiply(network->layers[i]->weights, network->layers[i]->neurons);
        matrixf_s *temp_neurons = matrix_add(multiplication_result, network->layers[i]->biases);
        for (int j = 0; j < network->layers[i + 1]->layer_size; j++) {
            network->layers[i + 1]->neurons->tab[j][0] = relu(temp_neurons->tab[j][0]);
        }
        matrixf_free(multiplication_result);
        matrixf_free(temp_neurons);
    }
}


void calculate_gradients(layer_s *prev_layer, layer_s *current_layer, dataset_s *dataset) {
    double d_activation = 0;
    double d_cost_d_neuron = 0; //dc/da

    for (int i = 0; i < current_layer->layer_size; i++) {
        for (int j = 0; j < prev_layer->layer_size; j++) {
            d_cost_d_neuron += 2 * (current_layer->neurons->tab[i][0] - dataset->expected_outputs->tab[i][0]);
            double tmp = prev_layer->neurons->tab[j][0];
            tmp *= prev_layer->weights->tab[i][j];
            d_activation += d_relu(tmp+ prev_layer->biases->tab[i][0]); //???

            current_layer->weights_cost_gradient->tab[i][0] += d_cost_d_neuron * d_activation * prev_layer->neurons->tab[j][0];
            current_layer->biases_cost_gradient->tab[i][0] += d_cost_d_neuron * d_activation;
        }
        d_activation = 0;
        d_cost_d_neuron = 0;
    }
}

void backpropagation(neural_network_s *network, dataset_s *dataset) {
    for (int i = network->layers_count - 1; i > 0; i--) {
        calculate_gradients(network->layers[i - 1], network->layers[i], dataset);
    }
}

void apply_gradients(neural_network_s *network, double learning_rate) {
    for (int i = 0; i < network->layers_count; i++) {
        for (int j = 0; j < network->layers[i]->next_layer_size; j++) {
            network->layers[i]->biases->tab[j][0] -= learning_rate * network->layers[i]->biases_cost_gradient->tab[j][0];
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                network->layers[i]->weights->tab[j][k] -= learning_rate * network->layers[i]->weights_cost_gradient->tab[j][k];
            }
        }
    }
}

void network_train(neural_network_s *network, dataset_s *dataset, int epochs, double learning_rate) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        feed_forward(network);
        //cost(network, dataset);
        backpropagation(network, dataset);
        apply_gradients(network, learning_rate);
    }
}

int main() {
    srand(time(NULL));

    dataset_s *dataset = load_data_csv("file.csv", 10, 10);
    printf("\n");
    matrixf_print(dataset->inputs, "dataset inputs");
    printf("\n");
    matrixf_print(dataset->expected_outputs, "dataset expected outputs");
    neural_network_s *network = create_neural_network(3, (int[]){10, 3, 2});
    network_train(network, dataset, 2, 0.1);

    free_dataset(dataset);
    free_neural_network(network);
    return 0;
}