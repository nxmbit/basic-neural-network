#include <stdio.h>
#include "network.h"
#include "matrixf_s.h"
#include "activations.h"
#include "dataset.h"
#include "training.h"

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

void calculate_gradients(layer_s *prev_layer, layer_s *current_layer, dataset_s *dataset, int sample_index) {
    double d_activation = 0;
    double d_cost_d_neuron = 0; //dc/da
//do poprawy
    for (int i = 0; i < current_layer->layer_size; i++) {
        for (int j = 0; j < prev_layer->layer_size; j++) {
            d_cost_d_neuron += 2 * (current_layer->neurons->tab[i][0] - dataset->expected_outputs->tab[sample_index][0]); //zle
            d_activation += d_relu(prev_layer->neurons->tab[j][0] * prev_layer->weights->tab[i][j] + prev_layer->biases->tab[i][0]); //???

            current_layer->weights_cost_gradient->tab[i][0] += d_cost_d_neuron * d_activation * prev_layer->neurons->tab[j][0];
            current_layer->biases_cost_gradient->tab[i][0] += d_cost_d_neuron * d_activation;
        }
        d_activation = 0;
        d_cost_d_neuron = 0;
    }
}

void backpropagation(neural_network_s *network, dataset_s *dataset, int sample_index) {
    for (int i = network->layers_count - 1; i > 0; i--) {
        calculate_gradients(network->layers[i - 1], network->layers[i], dataset, sample_index);
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
        double epoch_cost = 0;
        for (int sample_index = 0; sample_index < dataset->number_of_samples; sample_index++) {
            for (int i = 0; i < network->layers[0]->layer_size; i++) {
                network->layers[0]->neurons->tab[i][0] = dataset->inputs->tab[sample_index][i]; //copy from dataset to first layer
            }

            feed_forward(network);
            backpropagation(network, dataset, sample_index);
            apply_gradients(network, learning_rate);
            epoch_cost += cost(network, dataset, sample_index);
        }
        epoch_cost /= (double) dataset->number_of_samples;
        printf("Epoch: %d out of %d, cost: %lf\n", epoch + 1, epochs, epoch_cost);

    }
}

double cost(neural_network_s *network, dataset_s *dataset, int sample_index) { //MSE
    double cost = 0;
    for (int i = 0; i < network->layers_sizes[network->layers_count - 1]; i++) {
        double error = network->layers[network->layers_count - 1]->neurons->tab[i][0] - dataset->expected_outputs->tab[sample_index][0];
        cost += (error * error);
    }
    return (cost / (double) network->layers_sizes[network->layers_count]);
}