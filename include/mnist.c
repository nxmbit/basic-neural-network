#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"
#include "network.h"
#include "mnist.h"
#include "training.h"
#include <Python.h>


dataset_s *load_mnist(int number_of_samples, const char *path) {
    const int input_neurons = 784;
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
    getline(&line, &len, file); // skip first line (header)
    while ((read = getline(&line, &len, file)) != -1) {
        char *token = strtok(line, ",");
        int j = 0;
        while (token != NULL) {
            if (j == 0) {
                dataset->expected_outputs->tab[i][0] = atof(token);
            } else {
                dataset->inputs->tab[i][j] = atof(token);
                dataset->inputs->tab[i][j] /= 255.0;
            }
            token = strtok(NULL, ",");
            j++;
        }
        if (i == number_of_samples - 1) {
            break;
        }
        i++;
    }

    fclose(file);
    return dataset;
}

neural_network_s *train_mnist(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes) {
    dataset_s *dataset = load_mnist(60000, path);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    stochastic_network_train(network, dataset, epochs, learning_rate, batch_size);
    free_dataset(dataset);
    return network;
}

void mnist_draw(neural_network_s *network) {
    PyRun_SimpleFile(fopen("draw.py", "r"), "draw.py");
    const char* path = "image.csv";
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("Error while opening file %s\n", path);
        exit(1);
    }
    char row[1024];
    fgets(row, 1024, file);
    char *token = strtok(row, ",");
    int i = 0;
    while (token != NULL) {
        network->layers[0]->neurons->tab[i][0] = (atof(token)) / 255.0;
        token = strtok(NULL, ",");
        i++;
    }
    fclose(file);

    int max_index = 0;
    feed_forward(network);
    for (int j = 0; j < network->layers[network->layers_count - 1]->layer_size; j++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[j][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = j;
        }
    }
    printf("%d\n", max_index);
}
