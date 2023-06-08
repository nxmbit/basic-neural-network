#include <stdio.h>
#include "ui.h"
#include "user_io.h"
#include "mnist.h"
#include "logic_gates.h"
#include "network.h"
#include "color_classification.h"

void main_menu() {
    int choice = 0;
    while (choice != 3) {
        printf("1. Choose dataset to train on\n");
        printf("2. Load model\n");
        printf("3. Quit\n");
        choice = input_integer(1,3, "> ");
        switch (choice) {
            case 1:
                dataset_menu();
                break;
            case 2:
                load_model_menu();
                break;
            case 3:
            default:
                break;
        }
    }
}

void dataset_menu() {
    int choice = 0;
    while (choice != 5) {
        printf("Select dataset:\n");
        printf("1. Logic gates\n");
        printf("2. MNIST\n");
        printf("3. Color classification\n");
        printf("4. Custom dataset\n");
        printf("5. Back\n");
        choice = input_integer(1,5, "> ");
        switch (choice) {
            case 1:
                logic_gates_menu();
                break;
            case 2:
                mnist_menu(0, NULL);
                break;
            case 3:
                color_classification_menu(0, NULL);
                break;
            case 4:
                printf("Not implemented yet!\n");
                //custom_dataset_menu(); TODO: implement custom dataset support
                break;
            case 5:
            default:
                break;
        }
    }
}

void logic_gates_menu() {
    int choice = 0;
    while (choice != 6) {
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


void mnist_menu(int model_loaded, neural_network_s *model) {
    if (model_loaded) {
        int choice = 0;
        while (choice != 2) {
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
/*
void training_settings_menu() {
// TODO: custom training settings
}
*/

void load_model_menu() {
    neural_network_s *model = NULL;
    int choice = 0;
    while (choice != 3) {
        printf("1. Load MNIST model\n");
        printf("2. Load Color Classification model\n");
        printf("3. Back\n");
        choice = input_integer(1, 3, "> ");
        switch (choice) {
            case 1:
                model = load_model(MNIST_DEFAULT_MODEL_PATH);
                if (model == NULL) {
                    printf("Failed to load model!\n");
                } else {
                    printf("Model loaded!\n");
                    mnist_menu(1, model);
                    free_neural_network(model);
                }
                break;
            case 2:
                model = load_model(COLOR_DEFAULT_MODEL_PATH);
                if (model == NULL) {
                    printf("Failed to load model!\n");
                } else {
                    printf("Model loaded!\n");
                    color_classification_menu(1, model);
                    free_neural_network(model);
                }
                break;
            case 3:
            default:
                break;
        }
    }
}
