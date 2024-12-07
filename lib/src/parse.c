#include "lib/parse.h"

size_t parse_numbers(char const * string, array_t * destination) {
    char * next_ptr = string;
    char * end_ptr = string;

    size_t n_numbers = 0;
    while (true) {
        int32_t value = strtol(next_ptr, &end_ptr, 10);
        if (end_ptr == next_ptr) {
            break;
        }
        next_ptr = end_ptr;

        array_t_append(destination, &value);
        ++n_numbers;
    }

    return n_numbers;
}
