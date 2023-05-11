#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "dataset.h"

dataset_s *create_dataset(int input_neurons, int number_of_samples) {
    dataset_s *dataset = malloc(sizeof(dataset_s));
    dataset->inputs = create_matrix(number_of_samples, input_neurons);
    dataset->expected_outputs = create_matrix(number_of_samples, 1);
    dataset->number_of_samples = number_of_samples;
    dataset->input_neurons = input_neurons;
    return dataset;
}

void free_dataset(dataset_s *dataset) {
    matrixf_free(dataset->inputs);
    matrixf_free(dataset->expected_outputs);
    free(dataset);
}

dataset_s *load_data_csv(const char *path, int input_neurons, int number_of_samples) {
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
    while ((read = getline(&line, &len, file)) != -1) {
        char *token = strtok(line, ",");
        int j = 0;
        while (token != NULL) {
            if (j < input_neurons) {
                dataset->inputs->tab[i][j] = atof(token);
            } else {
                dataset->expected_outputs->tab[i][0] = atof(token);
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