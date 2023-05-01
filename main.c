#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

typedef struct matrixf_s {
    double **tab;
    int rows;
    int cols;
} matrixf_s;

matrixf_s *generate_struct(int rows, int cols) {
    matrixf_s *matrix = malloc(sizeof(matrixf_s));
    matrix->tab = malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix->tab[i] = malloc(cols * sizeof(double));
    }
    matrix->rows = rows;
    matrix->cols = cols;
    return matrix;
}

matrixf_s *matrix_multiply(const matrixf_s *matrix_1, const matrixf_s *matrix_2) {
    if (matrix_1->cols != matrix_2->rows) {
        printf("Matrix multiplication is not possible!\n");
    }
    matrixf_s *result_matrix = generate_struct(matrix_1->rows, matrix_2->cols);

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

void matrix_free(matrixf_s *matrix) {
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
    return 0;
};