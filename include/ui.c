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
