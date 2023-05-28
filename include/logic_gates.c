#include "logic_gates.h"
#include "dataset.h"
#include "network.h"
#include "training.h"

neural_network_s *train_logic_gates(const char *path, int epochs, double learning_rate, int *layers_sizes) {
    dataset_s *dataset = load_data_csv(path, 2, 4);
    neural_network_s *network = create_neural_network(2, layers_sizes);
    network_train(network, dataset, epochs, learning_rate);
    free_dataset(dataset);
    return network;
}

