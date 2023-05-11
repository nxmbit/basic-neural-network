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

void backpropagation(neural_network_s *network) {
    double d_activation = 0;
    double d_cost_d_neuron = 0;

    for (int i = network->layers_count - 1; i > 0; i--) {
        if (i == network->layers_count - 1) {
            for (int j = 0; j < network->layers[i]->layer_size; j++) {
                d_cost_d_neuron = 2 * (network->layers[i]->neurons->tab[j][0] - network->expected_output_neurons->tab[j][0]);
                printf("d_cost_d_neuron: %f\n", d_cost_d_neuron);
                for (int k = 0; k < network->layers[i - 1]->layer_size; k++) {
                    d_activation += d_relu(network->layers[i - 1]->neurons->tab[k][0] * network->layers[i - 1]->weights->tab[j][k] + network->layers[i - 1]->biases->tab[j][0]);
                    network->layers[i]->weights_cost_gradient->tab[j][0] += d_cost_d_neuron * d_activation * network->layers[i - 1]->neurons->tab[k][0];
                    network->layers[i]->biases_cost_gradient->tab[j][0] += d_cost_d_neuron * d_activation;
                }
                printf("d_activation: %f\n", d_activation);
                d_cost_d_neuron = 0;
                d_activation = 0;
            }
        } else {

        }

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

void set_expected_output_neurons(neural_network_s *network, dataset_s *dataset, int sample_index) {
    for (int i = 0; i < network->expected_output_neurons->rows; i++) {
        if (i == dataset->expected_outputs->tab[sample_index][0]) {
            network->expected_output_neurons->tab[i][0] = 1;
        } else {
            network->expected_output_neurons->tab[i][0] = 0;
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
            printf("\n");
            matrixf_print(network->layers[0]->neurons, "first layer");
            set_expected_output_neurons(network, dataset, sample_index);
            feed_forward(network);
            backpropagation(network);
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