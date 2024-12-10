#pragma once

#include <stdlib.h>

typedef struct matrix_t matrix_t;

matrix_t * matrix_t_new(size_t m, size_t n, size_t type_size);
void matrix_t_free(matrix_t * matrix);

size_t matrix_t_rows(matrix_t const * matrix);
size_t matrix_t_columns(matrix_t const * matrix);
size_t matrix_t_size(matrix_t const * matrix);

void matrix_t_set(matrix_t * matrix, size_t m, size_t n, void const * value);
void * matrix_t_at(matrix_t const * matrix, size_t m, size_t n);
