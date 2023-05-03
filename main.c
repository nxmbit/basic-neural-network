#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

typedef struct matrixf_s {
    double **tab;
    int rows;
    int cols;
} matrixf_s;

typedef struct layer_s {
    int layer_size;
    int next_layer_size;
    matrixf_s *neurons;
    matrixf_s *weights;
    matrixf_s *biases;
} layer_s;

typedef struct neural_network_s {
    layer_s **layers;
    int layers_count;
    int *layers_sizes;
    double cost;
} neural_network_s;

layer_s *create_layer(int layer_size, int next_layer_size);

matrixf_s *create_matrix(int rows, int cols);
matrixf_s *matrix_add(const matrixf_s *matrix_1, const matrixf_s *matrix_2);
void matrixf_free(matrixf_s *matrix);
void matrixf_print(const matrixf_s *matrix, const char *message);
matrixf_s *matrix_multiply(const matrixf_s *matrix_1, const matrixf_s *matrix_2);
void initialize_weights(neural_network_s *network);
double gaussian_noise_generator(double mean, double std_deviation);
neural_network_s *create_neural_network(int layers_count, int *layers_sizes);
void free_neural_network(neural_network_s *network);
void free_layer(layer_s *layer);
void initialize_biases(neural_network_s *network);

layer_s *create_layer(int layer_size, int next_layer_size) {
    layer_s *layer = malloc(sizeof(layer_s));
    layer->layer_size = layer_size;
    layer->next_layer_size = next_layer_size;
    layer->neurons = create_matrix(layer_size, 1);
    layer->weights = create_matrix(next_layer_size, layer_size);
    layer->biases = create_matrix(next_layer_size, 1);
    return layer;
}

void free_layer(layer_s *layer) {
    matrixf_free(layer->neurons);
    matrixf_free(layer->weights);
    matrixf_free(layer->biases);
    free(layer);
}

void free_neural_network(neural_network_s *network) {
    for (int i = 0; i< network->layers_count; i++) {
        free_layer(network->layers[i]);
    }
    free(network->layers);
    free(network->layers_sizes);
    free(network);
}

void feed_forward(neural_network_s *network)

//void cost(neural_network_s *network, )

 neural_network_s *create_neural_network(int layers_count, int *layers_sizes) {
    neural_network_s *network = malloc(sizeof(neural_network_s));
    network->layers = malloc(layers_count * sizeof(layer_s));
    network->layers_count = layers_count;
    network->layers_sizes = layers_sizes;
    for (int i = 0; i < layers_count; i++) {
        network->layers[i] = create_layer(layers_sizes[i], layers_sizes[i + 1]);
    }
    network->cost = 0;
    return network;
}

matrixf_s *create_matrix(int rows, int cols) {
    matrixf_s *matrix = malloc(sizeof(matrixf_s));
    matrix->tab = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix->tab[i] = malloc(cols * sizeof(double));
    }
    matrix->rows = rows;
    matrix->cols = cols;
    return matrix;
}

matrixf_s *matrix_add(const matrixf_s *matrix_1, const matrixf_s *matrix_2) {
    if ((matrix_1->rows != matrix_2->rows) || (matrix_1->cols != matrix_2->cols)) {
        printf("Matrix addition is not possible!\n");
    }
    matrixf_s *result_matrix = create_matrix(matrix_1->rows, matrix_1->cols);

    for (int i = 0; i < result_matrix->rows; i++) {
        for (int j = 0; j < result_matrix->cols; j++) {
            result_matrix->tab[i][j] = matrix_1->tab[i][j] + matrix_2->tab[i][j];
        }
    }
    return result_matrix;
}

matrixf_s *matrix_multiply(const matrixf_s *matrix_1, const matrixf_s *matrix_2) {
    if (matrix_1->cols != matrix_2->rows) {
        printf("Matrix multiplication is not possible!\n");
    }
    matrixf_s *result_matrix = create_matrix(matrix_1->rows, matrix_2->cols);

    for (int i = 0; i < matrix_1->rows; i++) {
        for (int j = 0; j < matrix_2->cols; j++) {
            result_matrix->tab[i][j] = 0;
            for (int k = 0; k < matrix_1->cols; k++) {
                result_matrix->tab[i][j] += (matrix_1->tab[i][k] * matrix_2->tab[k][j]);
            }
        }
    }
    return result_matrix;
}

void initialize_weights(neural_network_s *network) {
    for (int i = 0; i < network->layers_count; i++) {
        for (int j = 0; j < network->layers[i]->weights->rows; j++) {
            for (int k = 0; k < network->layers[i]->weights->cols; k++) {
                network->layers[i]->weights->tab[j][k] = gaussian_noise_generator(0, 1);
            }
        }
    }
}

void initialize_biases(neural_network_s * network) {
    for (int i = 0; i < network->layers_count; i++) {
        for (int j = 0; j < network->layers[i]->biases->rows; j++) {
            network->layers[i]->biases->tab[j][0] = 0;
        }
    }
}

double gaussian_noise_generator(double mean, double std_deviation) {
    srand48(time(NULL));
    double u1 = drand48();
    double u2 = drand48();
    //box muller transform
    double z0 = sqrt(-2.0 * log(u1)) * cos(2 * M_PI * u2);
    return (std_deviation * z0) + mean;
}

void feed_forward(neural_network_s *network) {
    matrixf_s *multiplication_result;
    for (int i = 1; i < network->layers_count; i++) {
        multiplication_result = matrix_multiply(network->layers[i]->weights, network->layers[i - 1]->neurons);
        network->layers[i]->neurons = matrix_add(multiplication_result, network->layers[i]->biases);
        for (int j = 0; j < network->layers[i]->layer_size; j++) {
            network->layers[i]->neurons->tab[j][0] = relu(network->layers[i]->neurons->tab[j][0]);
        }
        free(multiplication_result);
    }
}

void matrixf_free(matrixf_s *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->tab[i]);
    }
    free(matrix->tab);
    free(matrix);
}

void matrixf_print(const matrixf_s *matrix, const char *message) {
    printf("%s\n", message);
    for (int i = 0; i < matrix->rows; i++) {
        for (int j = 0; j < matrix->cols; j++) {
            printf("%lf ", matrix->tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

double relu(double x) {
    return fmax(0, x);
}

double d_relu (double x) {
    return x > 0 ? 1 : 0;
}

int main() {
    srand(time(NULL));
    return 0;
}