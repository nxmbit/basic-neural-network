#ifndef DATASET_H
#define DATASET_H

#include "matrixf_s.h"

typedef struct dataset_s {
    matrixf_s *inputs;
    matrixf_s *expected_outputs;
    int number_of_samples;
    int input_neurons;
    int output_neurons;
} dataset_s;

dataset_s *create_dataset(int input_neurons, int number_of_samples);
void free_dataset(dataset_s *dataset);
dataset_s *load_data_csv(const char *path, int input_neurons, int number_of_samples);

#endif