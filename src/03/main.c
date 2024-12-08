#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT

#ifdef EXAMPLE
#define FILENAME "example.txt"
#define LINE_LENGTH 84
#elifdef INPUT
#define FILENAME "input.txt"
#define LINE_LENGTH 6400
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one(FILENAME));
    printf("Part Two: %" PRIi32 "\n", part_two(FILENAME));
    return 0;
}

int32_t parse_mul(char * string, char ** end_ptr) {
    char * arg_1_ptr = strstr(string, "mul(") + 4;
    if (arg_1_ptr - 4 == NULL) {
        *end_ptr = string;
        return 0;
    }

    char * arg_2_ptr = strchr(arg_1_ptr, ',') + 1;
    if (arg_2_ptr - 1 == nullptr) {
        *end_ptr = string;
        return 0;
    }

    char * cmd_end_ptr = strchr(arg_2_ptr, ')');
    if (cmd_end_ptr == nullptr) {
        *end_ptr = string;
        return 0;
    }

    char * end = arg_1_ptr;
    int32_t arg_1 = strtol(arg_1_ptr, &end, 10);
    if (end != arg_2_ptr - 1) {
        *end_ptr = string + 1;
        return 0;
    }

    int32_t arg_2 = strtol(arg_2_ptr, &end, 10);
    if (end != cmd_end_ptr) {
        *end_ptr = string + 1;
        return 0;
    }

    *end_ptr = cmd_end_ptr;
    return arg_1 * arg_2;
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t sum = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        char * start_ptr = buffer;
        char * end_ptr = start_ptr;
        while (true) {
            int32_t result = parse_mul(start_ptr, &end_ptr);
            sum += result;
            if (end_ptr == start_ptr) {
                break;
            }

            start_ptr = end_ptr;
        }
    }

    return sum;
}

int32_t part_two(char const * filename) {
    return 0;
}