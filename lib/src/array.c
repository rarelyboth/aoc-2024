#include "lib/array.h"

#include <assert.h>
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

void array_t_resize(array_t * array, size_t capacity) {
    assert(array && array->data);
    assert(capacity >= 0);

    array->data = (char *)realloc(array->data, capacity * array->size);
    assert(array->data);
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

void * array_t_begin(array_t const * array) {
    return array_t_at(array, 0);
}

void * array_t_end(array_t const *array) {
    return array->data + array->length * array->size;
}
