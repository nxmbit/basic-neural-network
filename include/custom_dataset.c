#include <stdio.h>
#include <string.h>
#include "dataset.h"
#include "training.h"
#include "color_classification.h"
#include "custom_dataset.h"

neural_network_s *train_custom_dataset(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes) {
    dataset_s *dataset = load_data_csv(path, 3, 70000);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    stochastic_network_train(network, dataset, epochs, learning_rate, batch_size);
    free_dataset(dataset);
    return network;
}
