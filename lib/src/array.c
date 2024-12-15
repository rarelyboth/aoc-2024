#include "lib/array.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GROWTH_FACTOR 2

struct array_t {
    size_t capacity;
    size_t length;
    size_t size;
    char * data;
};

array_t * array_t_new(size_t type_size) {
    array_t * array = (array_t *)malloc(sizeof(array_t));
    assert(array);

    array->capacity = 1;
    array->length = 0;
    array->size = type_size;
    array->data = (char *)calloc(array->capacity, array->size);
    assert(array->data);

    return array;
}

void array_t_free(array_t * array) {
    assert(array && array->data);
    free(array->data);
    free(array);
}

size_t array_t_capacity(array_t const * array) {
    return array->capacity;
}

size_t array_t_length(array_t const * array) {
    return array->length;
}

size_t array_t_size(array_t const * array) {
    return array->size;
}

bool array_t_empty(array_t const *array) {
    return array->length == 0;
}

void array_t_resize(array_t * array, size_t capacity) {
    assert(array && array->data);
    assert(capacity >= 0);

    array->data = (char *)realloc(array->data, capacity * array->size);
    assert(array->data);

    array->capacity = capacity;
}

void array_t_append(array_t * array, void const * value) {
    if (array->length >= array->capacity) {
        array_t_resize(array, array->length * GROWTH_FACTOR);
    }

    memcpy(array->data + array->length * array->size, value, array->size);
    ++array->length;
}

void * array_t_at(array_t const * array, size_t index) {
    assert(index < array->length);

    return array->data + index * array->size;
}

array_t * array_t_copy(array_t const * array) {
    array_t * copy = array_t_new(array_t_size(array));
    array_t_resize(copy, array_t_length(array));

    memcpy(copy->data, array->data, array->length * array->size);
    copy->length = array_t_length(array);

    return copy;
}

void array_t_sort(array_t *array, int (*compare_fn)(void const *, void const *)) {
    qsort(array->data, array->length, array->size, compare_fn);
}

void * array_t_linear_find(array_t const * array, void const * value, bool(*equality_fn)(void const *, void const *)) {
    for (void const * it = (void const *)array_t_begin(array); it != array_t_end(array); it += array->size) {
        if (equality_fn(it, value)) {
            return it;
        }
    }

    return array_t_end(array);
}

void * array_t_iterator_next(array_t const * array, void * iterator) {
    return iterator + array->size;
}

void array_t_accumulate(array_t const * array, void * result, void (*fn)(void *, void const *)) {
    for (void * it = array_t_begin(array); it != array_t_end(array); it += array->size) {
        fn(result, it);
    }
}

void array_t_remove(array_t * array, size_t index) {
    memmove(
        array->data + index * array->size,
        array->data + (index + 1) * array->size,
        (array->length - index - 1) * array->size
    );
    --array->length;
}

void * array_t_begin(array_t const * array) {
    if (array_t_length(array) == 0) {
        return array_t_end(array);
    }

    return array_t_at(array, 0);
}

void * array_t_end(array_t const *array) {
    return array->data + array->length * array->size;
}
