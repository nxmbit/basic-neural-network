#ifndef MATRIX_S_H
#define MATRIX_S_H

typedef struct matrix_s {
    int **tab;
    int rows;
    int cols;
} matrix_s;

matrix_s *create_matrix(int rows, int cols);
void matrix_free(matrix_s *matrix);

#endif
