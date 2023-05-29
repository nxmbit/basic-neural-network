#include <stdio.h>
#include <Python.h>
#include <string.h>
#include "dataset.h"
#include "training.h"
#include "color_classification.h"

neural_network_s *train_color_classification(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes) {
    dataset_s *dataset = load_data_csv(path, 3, 70000);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    stochastic_network_train(network, dataset, epochs, learning_rate, batch_size);
    free_dataset(dataset);
    return network;
}

void select_color(neural_network_s *network) {
    PyRun_SimpleFile(fopen("select_color.py", "r"), "select_color.py");
    const char* path = "image.csv";
    FILE *file = fopen(path, "r"); //TODO: can split it into separate function, the code is the same as in mnist_draw
    if (file == NULL) {
        printf("Error while opening file %s\n", path);
        exit(1);
    }
    char row[1024];
    fgets(row, 1024, file);
    char *token = strtok(row, ",");
    int i = 0;
    while (token != NULL) {
        network->layers[0]->neurons->tab[0][i] = (atof(token)) / 255.0;
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