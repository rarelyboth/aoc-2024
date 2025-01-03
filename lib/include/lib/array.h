#pragma once

#include <stdlib.h>

typedef struct array_t array_t;

array_t * array_t_new(size_t type_size);
void array_t_free(array_t * array);

size_t array_t_capacity(array_t const * array);
size_t array_t_length(array_t const * array);
size_t array_t_size(array_t const * array);
bool array_t_empty(array_t const * array);

void array_t_resize(array_t * array, size_t capacity);

void array_t_append(array_t * array, void const * value);
void * array_t_at(array_t const * array, size_t index);
array_t * array_t_copy(array_t const * array);
void array_t_sort(array_t * array, int (*compare_fn)(void const *, void const *));
void * array_t_linear_find(array_t const * array, void const * value, bool (*equality_fn)(void const *, void const *));
void * array_t_binary_find(array_t const * array, void const * value, int (*compare_fn)(void const *, void const *));
void array_t_accumulate(array_t const * array, void * result, void (*fn)(void *, void const *));

void array_t_remove(array_t * array, size_t index);

void * array_t_begin(array_t const * array);
void * array_t_end(array_t const * array);
void * array_t_rbegin(array_t const * array);
void * array_t_rend(array_t const * array);