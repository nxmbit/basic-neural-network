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
            if (i == network->layers_count - 2) {
                network->layers[i + 1]->neurons->tab[j][0] = softmax(temp_neurons->tab[j][0], temp_neurons->tab, network->layers[i + 1]->layer_size);
            } else {
                network->layers[i + 1]->neurons->tab[j][0] = sigmoid(temp_neurons->tab[j][0]);
            }
        }
        matrixf_free(multiplication_result);
        matrixf_free(temp_neurons);
    }
}

void backpropagation(neural_network_s *network) {
    for (int i = network->layers_count - 1; i > 0; i--) {
        double error = 0;
        matrixf_s *multiplication_result; //neuron * weight
        matrixf_s *multiplication_result_2; // weight * delta
        matrixf_s *subtraction_result;
        matrixf_s *trans;
        matrixf_s *temp_neurons;
        multiplication_result = matrix_multiply(network->layers[i - 1]->weights, network->layers[i - 1]->neurons);
        temp_neurons = matrix_add(multiplication_result, network->layers[i - 1]->biases);
        if (i == network->layers_count - 1) {
            subtraction_result = matrix_subtract(network->layers[i]->neurons,network->expected_output_neurons);
            for (int j = 0; j < network->layers[i]->layer_size; j++) {
                error = subtraction_result->tab[j][0];
                subtraction_result->tab[j][0] = d_softmax(temp_neurons->tab[j][0], temp_neurons->tab, temp_neurons->rows) * error;
            }
            matrix_copy(network->layers[i - 1]->neurons_delta, subtraction_result);
            matrixf_free(subtraction_result);
            matrixf_free(multiplication_result);
            matrixf_free(temp_neurons);
        } else {
            trans = matrix_transpose(network->layers[i]->weights);
            multiplication_result_2 = matrix_multiply(trans, network->layers[i]->neurons_delta);
            for (int j = 0; j < network->layers[i]->layer_size; j++) {
                error = multiplication_result_2->tab[j][0];
                network->layers[i - 1]->neurons_delta->tab[j][0] = d_sigmoid(temp_neurons->tab[j][0]) * error;

            }
            matrixf_free(multiplication_result);
            matrixf_free(multiplication_result_2);
            matrixf_free(temp_neurons);
            matrixf_free(trans);
        }
    }

    for (int i = 0; i < network->layers_count - 1; i++) {
        matrixf_s *trans = matrix_transpose(network->layers[i]->neurons);
        matrixf_s *multiplication_result = matrix_multiply(network->layers[i]->neurons_delta, trans);
        matrix_copy(network->layers[i]->biases_gradient, network->layers[i]->neurons_delta);
        matrix_copy(network->layers[i]->weights_gradient, multiplication_result);
        matrixf_free(trans);
        matrixf_free(multiplication_result);
    }
}

void apply_gradients(neural_network_s *network, double learning_rate) {
    for (int i = 0; i < network->layers_count - 1; i++) {
        for (int j = 0; j < network->layers[i]->next_layer_size; j++) {
            network->layers[i]->biases->tab[j][0] -= learning_rate * network->layers[i]->biases_gradient->tab[j][0];
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                network->layers[i]->weights->tab[j][k] -= learning_rate * network->layers[i]->weights_gradient->tab[j][k];
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
        double epoch_success_rate = 0;
        for (int sample_index = 0; sample_index < dataset->number_of_samples; sample_index++) {
            for (int i = 0; i < network->layers[0]->layer_size; i++) {
                network->layers[0]->neurons->tab[i][0] = dataset->inputs->tab[sample_index][i]; //copy from dataset to first layer
            }
            set_expected_output_neurons(network, dataset, sample_index);
            feed_forward(network);
            backpropagation(network);
            apply_gradients(network, learning_rate);
            epoch_cost += cost(network);
            epoch_success_rate += success_rate(network, sample_index, dataset);
        }
        epoch_cost /= (double) dataset->number_of_samples;
        epoch_success_rate /= (double) dataset->number_of_samples;
        printf("Epoch: %d out of %d, cost: %lf, success rate: %lf\n", epoch + 1, epochs, epoch_cost, epoch_success_rate);

    }
}

double success_rate(neural_network_s *network, int sample_index, dataset_s *dataset) {
    double success = 0;
    int max_index = 0;
    for (int i = 0; i < network->layers[network->layers_count - 1]->layer_size; i++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[i][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = i;
        }
    }
    if (max_index == dataset->expected_outputs->tab[sample_index][0]) {
        success = 1;
    }
    return success;
}

double cost(neural_network_s *network) { //MSE
    double cost = 0;
    for (int i = 0; i < network->layers_sizes[network->layers_count - 1]; i++) {
        double error = network->layers[network->layers_count - 1]->neurons->tab[i][0] - network->expected_output_neurons->tab[i][0];
        cost += (error * error);
    }
    return (cost / (double) network->layers_sizes[network->layers_count - 1]);
}

void result(neural_network_s *network) {
    double val1, val2, val3;
    int max_index = 0;
    scanf("%lf %lf %lf", &val1, &val2, &val3);
    network->layers[0]->neurons->tab[0][0] = val1;
    network->layers[0]->neurons->tab[1][0] = val2;
    network->layers[0]->neurons->tab[2][0] = val3;
    feed_forward(network);
    for (int i = 0; i < network->layers[network->layers_count - 1]->layer_size; i++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[i][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = i;
        }
    }
    printf("%d\n", max_index);
}