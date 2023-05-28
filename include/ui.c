#include <stdio.h>
#include "ui.h"
#include "user_io.h"
#include "mnist.h"
#include "logic_gates.h"
#include "network.h"

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
        choice = input_integer(1,3, "> ");
        switch (choice) {
            case 1:
                logic_gates_menu();
                break;
            case 2:
                mnist_menu();
                break;
            case 3:
                //color_classification_menu();
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
    while (choice != 5) {
        printf("Select logic gate:\n");
        printf("1. AND\n");
        printf("2. OR\n");
        printf("3. XOR\n");
        printf("4. NAND\n");
        printf("5. Back\n");
        choice = input_integer(1,5, "> ");
        switch (choice) {
            case 1:
                train_logic_gates("and.csv", 1000, 0.01, (int[]){2,10,2});
                break;
            case 2:
                train_logic_gates("or.csv", 1000, 0.01, (int[]){2,10,2});
                break;
            case 3:
                train_logic_gates("xor.csv", 1000, 0.01, (int[]){2,10,2});
                break;
            case 4:
                train_logic_gates("nand.csv", 1000, 0.01, (int[]){2,10,2});
                break;
            case 5:
            default:
                break;
        }
    }
}


void mnist_menu() {
    neural_network_s *network = train_mnist("mnist_train.csv", 10000, 0.01, 1000, (int[]){784,128,48,10});
    int choice = 0;
    while (choice != 2) {
        printf("1. Draw a number\n");
        printf("2. Back\n");
        choice = input_integer(1,2, "> ");
        switch (choice) {
            case 1:
                mnist_draw(network);
                break;
            case 2:
            default:
                break;
        }
    }
    free_neural_network(network);
}


/*
void training_settings_menu() {
// TODO: custom training settings
}
*/

void load_model_menu() {


}
