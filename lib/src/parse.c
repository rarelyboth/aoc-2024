#include "lib/parse.h"

#include <stdint.h>
#include <string.h>

#define DIGITS "0123456789"

size_t parse_i32_numbers(char const * string, array_t * destination) {
    char * next_ptr = strpbrk(string, DIGITS);
    char * end_ptr = nullptr;

    size_t n_numbers = 0;
    while (next_ptr != nullptr) {
        int32_t value = strtol(next_ptr, &end_ptr, 10);
        if (end_ptr == next_ptr) {
            break;
        }
        array_t_append(destination, &value);

        ++n_numbers;

        next_ptr = strpbrk(end_ptr, DIGITS);
    }

    return n_numbers;
}

size_t parse_u64_numbers(char const * string, array_t * destination) {
    char * next_ptr = strpbrk(string, DIGITS);
    char * end_ptr = nullptr;

    size_t n_numbers = 0;
    while (next_ptr != nullptr) {
        uint64_t value = strtol(next_ptr, &end_ptr, 10);
        if (end_ptr == next_ptr) {
            break;
        }
        array_t_append(destination, &value);

        ++n_numbers;

        next_ptr = strpbrk(end_ptr, DIGITS);
    }

    return n_numbers;
}