#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "dataset.h"
#include "training.h"
#include "user_io.h"
#include "limits.h"
#include "network.h"
#include "color_classification.h"
#include "custom_dataset.h"

void custom_dataset_menu(int model_loaded, neural_network_s *model) {
    if (model_loaded) {
        int choice = 0;
        while (choice != 2) {
            printf("1. Test dataset\n");
            printf("2. Back\n");
            choice = input_integer(1, 2, "> ");
            switch (choice) {
                case 1:
                    test_custom_dataset(model);
                    break;
                case 2:
                default:
                    break;
            }
        }
    } else {
        const char *custom_dataset_model_folder = "../models/custom/";
        const char *custom_dataset_folder = "../datasets/custom/";

        char *path = input_string(128,"Enter dataset name: ");
        char *dataset_path = malloc(sizeof(char) * (strlen(custom_dataset_folder) + strlen(path) + 1));
        strcpy(dataset_path, custom_dataset_folder);
        strcat(dataset_path, path);

        int samples_num = input_integer(1, INT_MAX, "Enter number of samples: ");
        int batch_size = input_integer(1, samples_num, "Enter batch size: ");
        int number_of_epochs = input_integer(1, INT_MAX, "Enter number of epochs: ");
        int layers_count = input_integer(1, 100, "Enter number of layers: ");
        int *layers_sizes = malloc(sizeof(int) * layers_count);
        for (int i = 0; i < layers_count; i++) {
            printf("Enter size of layer %d: ", i + 1);
            layers_sizes[i] = input_integer(1, INT_MAX, "");
        }

        neural_network_s *network = train_custom_dataset(dataset_path, number_of_epochs, 0.01, batch_size, layers_count, layers_sizes, samples_num);

        int choice = 0;
        while (choice != 3) {
            printf("1. Test dataset\n");
            printf("2. Save model\n");
            printf("3. Back\n");
            choice = input_integer(1, 3, "> ");
            switch (choice) {
                case 1:
                    test_custom_dataset(network);
                    break;
                case 2:
                    char *model_name = input_string(128, "Enter model name: ");
                    char *model_path = malloc(sizeof(char) * (strlen(model_name) + strlen(custom_dataset_model_folder) + 1));
                    strcpy(model_path, custom_dataset_model_folder);
                    strcat(model_path, model_name);
                    save_model(network, model_path);
                    printf("Model saved!\n");
                    free(model_path);
                    break;
                case 3:
                default:
                    break;
            }
            free_neural_network(network);
        }
        free(layers_sizes);
        free(dataset_path);
    }
}

neural_network_s *train_custom_dataset(const char *path, int epochs, double learning_rate, int batch_size, int layers_count, int *layers_sizes, int samples_count) {
    dataset_s *dataset = load_data_csv(path, layers_sizes[0], samples_count);
    neural_network_s *network = create_neural_network(layers_count, layers_sizes);
    stochastic_network_train(network, dataset, epochs, learning_rate, batch_size);
    free_dataset(dataset);
    return network;
}

void test_custom_dataset(neural_network_s *network) {
    for (int i = 0; i < network->layers[0]->layer_size; i++) {
        printf("Enter value for input %d: ", i + 1);
        double test;
        scanf("%lf", &test);
        network->layers[0]->neurons->tab[i][0] = test;
    }
    int max_index = 0;
    feed_forward(network);
    for (int j = 0; j < network->layers[network->layers_count - 1]->layer_size; j++) {
        if (network->layers[network->layers_count - 1]->neurons->tab[j][0] > network->layers[network->layers_count - 1]->neurons->tab[max_index][0]) {
            max_index = j;
        }
    }
    printf("Result: %d\n", max_index);
}