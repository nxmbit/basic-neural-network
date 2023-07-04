#include <stdio.h>
#include "logic_gates.h"
#include "dataset.h"
#include "network.h"
#include "training.h"
#include "user_io.h"

void logic_gates_menu() {
    int choice = 0;
    while (choice != 6) {
        clrscr();
        printf("Select logic gate:\n");
        printf("1. AND\n");
        printf("2. OR\n");
        printf("3. XOR\n");
        printf("4. NAND\n");
        printf("5. NOR\n");
        printf("6. Back\n");
        choice = input_integer(1,6, "> ");
        switch (choice) {
            case 1:
                logic_gates_training_menu(AND_PATH, 10000, 0.1, 3, (int[]) {2, 5, 2}, "AND");
                break;
            case 2:
                logic_gates_training_menu(OR_PATH, 10000, 0.1, 3, (int[]) {2, 5, 2}, "OR");
                break;
            case 3:
                logic_gates_training_menu(XOR_PATH, 10000, 0.1, 3, (int[]) {2, 5, 2}, "XOR");
                break;
            case 4:
                logic_gates_training_menu(NAND_PATH, 10000, 0.1, 3, (int[]) {2, 5, 2}, "NAND");
                break;
            case 5:
                logic_gates_training_menu(NOR_PATH, 10000, 0.1, 3, (int[]) {2, 5, 2}, "OR");
                break;
            case 6:
            default:
                break;
        }
    }
}

void logic_gates_training_menu(const char *def_path, int def_epochs, double def_learning_rate, int def_number_of_layers, int *def_layers_sizes, const char *gate_name) {
    neural_network_s *network = train_logic_gates(def_path, def_epochs, def_learning_rate, def_number_of_layers, def_layers_sizes);
    int choice = 0;
    int val1, val2;
    while (choice != 2) {
        clrscr();
        printf("1. Test %s gate\n", gate_name);
        printf("2. Back\n");
        choice = input_integer(1,2, "> ");
        switch (choice) {
            case 1:
                val1 = input_integer(0,1, "Enter first value: ");
                val2 = input_integer(0,1, "Enter second value: ");
                logic_gates_test(network, val1, val2);
                break;
            case 2:
            default:
                break;
        }
    }
    free_neural_network(network);
}

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
    printf("Result: %d\n\n", max_index);
}