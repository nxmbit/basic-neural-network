#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataset.h"
#include "network.h"
#include "mnist.h"
#include "training.h"
#include "user_io.h"
#include <Python.h>

void mnist_menu(int model_loaded, neural_network_s *model) {
    if (model_loaded) {
        int choice = 0;
        while (choice != 2) {
            clrscr();
            printf("1. Draw a number\n");
            printf("2. Back\n");
            choice = input_integer(1,2, "> ");
            switch (choice) {
                case 1:
                    mnist_draw(model);
                    break;
                case 2:
                default:
                    break;
            }
        }
    } else {
        neural_network_s *network = train_mnist(MNIST_DATASET_PATH, 1000, 0.05, 300, 4, (int[]){784,200,72,10});
        int choice = 0;
        while (choice != 3) {
            clrscr();
            printf("1. Draw a number\n");
            printf("2. Save model\n");
            printf("3. Back\n");
            choice = input_integer(1,3, "> ");
            switch (choice) {
                case 1:
                    mnist_draw(network);
                    break;
                case 2:
                    save_model(network, MNIST_DEFAULT_MODEL_PATH);
                    printf("Model saved!\n");
                    break;
                case 3:
                default:
                    break;
            }
        }
        free_neural_network(network);
    }
}

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
    PyRun_SimpleFile(fopen(MNIST_DRAW, "r"), "draw.py");
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
    printf("Result: %d\n\n", max_index);
}
