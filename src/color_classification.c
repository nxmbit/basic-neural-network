#include <stdio.h>
#include <Python.h>
#include <string.h>
#include "dataset.h"
#include "training.h"
#include "user_io.h"
#include "color_classification.h"

void color_classification_menu(int model_loaded, neural_network_s *model) {
    if (model_loaded) {
        int choice = 0;
        while (choice != 2) {
            printf("1. Select color\n");
            printf("2. Back\n");
            choice = input_integer(1, 2, "> ");
            switch (choice) {
                case 1:
                    select_color(model);
                    break;
                case 2:
                default:
                    break;
            }
        }
    } else {
        neural_network_s *network = train_color_classification(COLOR_DATASET_PATH, 10000, 0.01, 100, 4,
                                                               (int[]) {3, 10, 20, 16});
        int choice = 0;
        while (choice != 3) {
            printf("1. Test color classification\n");
            printf("2. Save model\n");
            printf("3. Back\n");
            choice = input_integer(1, 3, "> ");
            switch (choice) {
                case 1:
                    select_color(network);
                    break;
                case 2:
                    save_model(network, COLOR_DEFAULT_MODEL_PATH);
                    printf("Model saved!\n");
                    break;
                case 3:
                default:
                    break;
            }
            free_neural_network(network);
        }
    }
}

neural_network_s *train_color_classification(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes) {
    dataset_s *dataset = load_data_csv(path, 3, 70000);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    stochastic_network_train(network, dataset, epochs, learning_rate, batch_size);
    free_dataset(dataset);
    return network;
}

void select_color(neural_network_s *network) {
    PyRun_SimpleFile(fopen(COLOR_SELECTOR_PATH, "r"), "select_color.py");
    const char* path = "color_cielab.csv";
    const char labels[16][15] = {"White", "Gray", "Black", "Red", "Pink", "Dark Red", "Orange", "Brown",
                                 "Yellow", "Green", "Dark Green", "Teal", "Light Blue", "Blue", "Dark Blue", "Purple"};
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
        network->layers[0]->neurons->tab[i][0] = (atof(token));
        token = strtok(NULL, ",");
        i++;
    }
    fclose(file);

    //normalize cielab data
    network->layers[0]->neurons->tab[0][0] /= 100.0;
    network->layers[0]->neurons->tab[1][0] /= 128.0;
    network->layers[0]->neurons->tab[2][0] /= 128.0;

    int max_index = 0;
    feed_forward(network);
    for (int j = 0; j < network->layers[network->layers_count - 1]->layer_size; j++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[j][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = j;
        }
    }
    printf("Color Name: %s\n", labels[max_index]);
}