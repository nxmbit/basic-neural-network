#include <stdio.h>
#include <malloc.h>
#include "matrixf_s.h"

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

matrixf_s *matrix_subtract(const matrixf_s *matrix_1, const matrixf_s *matrix_2) {
    if ((matrix_1->rows != matrix_2->rows) || (matrix_1->cols != matrix_2->cols)) {
        printf("Matrix subtraction is not possible!\n");
    }
    matrixf_s *result_matrix = create_matrix(matrix_1->rows, matrix_1->cols);

    for (int i = 0; i < result_matrix->rows; i++) {
        for (int j = 0; j < result_matrix->cols; j++) {
            result_matrix->tab[i][j] = matrix_1->tab[i][j] - matrix_2->tab[i][j];
        }
    }
    return result_matrix;
}

matrixf_s *matrix_multiply(const matrixf_s *matrix_1, const matrixf_s *matrix_2) {
    if (matrix_1->cols != matrix_2->rows) {
        printf("Matrix multiplication is not possible!\n");
        return NULL;
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

matrixf_s *matrix_square_elements(const matrixf_s *matrix) {
    matrixf_s *result_matrix = create_matrix(matrix->rows, matrix->cols);

    for (int i = 0; i < result_matrix->rows; i++) {
        for (int j = 0; j < result_matrix->cols; j++) {
            result_matrix->tab[i][j] = matrix->tab[i][j] * matrix->tab[i][j];
        }
    }
    return result_matrix;
}

matrixf_s *split_matrix_column(const matrixf_s *matrix, int n) {
    matrixf_s *result = create_matrix(matrix->rows, 1);
    for (int i = 0; i < matrix->rows; i++) {
        result->tab[i][0] = matrix->tab[i][n];
    }
    return result;
}

double matrixf_column_sum(const matrixf_s *matrix, int n) {
    double sum = 0;
    for (int i = 0; i < matrix->rows; i++) {
        sum += matrix->tab[i][n];
    }
    return sum;
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