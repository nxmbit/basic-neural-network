#include <malloc.h>
#include "matrix_s.h"

matrix_s *create_matrix(int rows, int cols) {
    matrix_s *matrix = malloc(sizeof(matrix_s));
    matrix->tab = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        matrix->tab[i] = malloc(cols * sizeof(int));
    }
    matrix->rows = rows;
    matrix->cols = cols;
    return matrix;
}

void matrix_free(matrix_s *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->tab[i]);
    }
    free(matrix->tab);
    free(matrix);
}