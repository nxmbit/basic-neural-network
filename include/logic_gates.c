#include <stdio.h>
#include "logic_gates.h"
#include "dataset.h"
#include "network.h"
#include "training.h"

neural_network_s *train_logic_gates(const char *path, int epochs, double learning_rate, int layers_count,  int *layers_sizes) {
    dataset_s *dataset = load_data_csv(path, 2, 4);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    network_train(network, dataset, epochs, learning_rate);
    free_dataset(dataset);
    return network;
}

void logic_gates_test(neural_network_s *network, int val1, int val2) {
    network->layers[0]->neurons->tab[0][0] = val1;
    network->layers[0]->neurons->tab[1][0] = val2;
    int max_index = 0;
    feed_forward(network);
    for (int j = 0; j < network->layers[network->layers_count - 1]->layer_size; j++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[j][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = j;
        }
    }
    printf("Result: %d\n", max_index);
}