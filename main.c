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


int main() {
    return 0;
};