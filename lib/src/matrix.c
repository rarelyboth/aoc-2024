#include "lib/matrix.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct matrix_t {
    size_t rows;
    size_t columns;
    size_t size;
    char * data;
};

matrix_t * matrix_t_new(size_t m, size_t n, size_t type_size) {
    matrix_t * matrix = (matrix_t *)malloc(sizeof(matrix_t));
    assert(matrix);

    matrix->rows = m;
    matrix->columns = n;
    matrix->size = type_size;

    matrix->data = (char *)calloc(m * n, type_size);
    assert(matrix->data);

    return matrix;
}

void matrix_t_free(matrix_t * matrix) {
    assert(matrix && matrix->data);

    free(matrix->data);
    free(matrix);
}

size_t matrix_t_rows(matrix_t const * matrix) {
    assert(matrix);

    return matrix->rows;
}

size_t matrix_t_columns(matrix_t const * matrix) {
    assert(matrix);

    return matrix->columns;
}

size_t matrix_t_size(matrix_t const * matrix) {
    return matrix->size;
}

void matrix_t_set(matrix_t * matrix, size_t m, size_t n, void const * value) {
    assert(matrix && matrix->data);
    assert(m < matrix->rows);
    assert(n < matrix->columns);

    memcpy(
        matrix->data + (m * matrix_t_columns(matrix) * matrix_t_size(matrix)) + (n * matrix_t_size(matrix)),
        value,
        matrix->size
    );
}

void * matrix_t_at(matrix_t const *matrix, size_t m, size_t n) {
    assert(matrix && matrix->data);
    assert(m < matrix->rows);
    assert(n < matrix->columns);

    return matrix->data + (m * matrix_t_columns(matrix) * matrix_t_size(matrix)) + (n * matrix_t_size(matrix));
}


