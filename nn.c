#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/matrixf_s.h"
#include "include/network.h"
#include "include/dataset.h"
#include "include/training.h"
#include "include/mnist.h"
#include <string.h>
#include "include/ui.h"
#include <Python.h>

int main(int argc, char *argv[]) {
    Py_Initialize();
    srand(time(NULL));
    //dataset_s *dataset = load_data_csv("colors_cielab.csv", 3, 70000);
    //dataset_s *dataset = load_mnist(60000, "mnist_train.csv");
    //neural_network_s *network = create_neural_network(4, (int[]){3,10,20,16});
    //neural_network_s *network = create_neural_network(4, (int[]){784,128,48,10});
    //stochastic_network_train(network, dataset, 10000, 0.01, 1000);
    //stochastic_network_train(network, dataset, 10000, 0.05, 100);
    //neural_network_s *network = load_model("model.txt");
    //network_train(network, dataset, 100, 0.01);

    //save_model(network, "model.txt");
    //result(network);
    main_menu();

    Py_Finalize();
    return 0;
}