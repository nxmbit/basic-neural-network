#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "include/matrixf_s.h"
#include "include/network.h"
#include "include/dataset.h"
#include "include/training.h"
#include <string.h>

int main() {
    srand(time(NULL));
    dataset_s *dataset = load_data_csv("training_data.csv", 3, 70000);
    neural_network_s *network = create_neural_network(4, (int[]){3,10,16, 20,16});
    //neural_network_s *network = load_model("model.txt");
    network_train(network, dataset, 400, 0.1);
    save_model(network, "model.txt");
    result(network);
    matrixf_print(network->layers[network->layers_count - 1]->neurons, "last layer");
    free_dataset(dataset);
    free_neural_network(network);
    return 0;
}