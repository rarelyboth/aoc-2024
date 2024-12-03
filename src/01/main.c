#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIGITS "0123456789"

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one("input.txt"));
    printf("Part Two: %" PRIi32 "\n", part_two("input.txt"));
    return 0;
}

int32_t compare_ints(void const * first_, void const * second_) {
    int32_t first = *(int32_t const *)first_;
    int32_t second = *(int32_t const *)second_;

    if (first == second) {
        return 0;
    }
    return first < second ? -1 : 1;
}

int32_t min(int32_t first, int32_t second) {
    if (first < second) {
        return first;
    }

    return second;
}

int32_t max(int32_t first, int32_t second) {
    if (first > second) {
        return first;
    }

    return second;
}

void read_location_ids(char const * filename, int32_t first_ids[1000], int32_t second_ids[1000]) {
    FILE * file = file_open(filename, "r");

    size_t constexpr buffer_size = 16;
    char buffer[buffer_size] = {};

    for (size_t line_no = 0; fgets(buffer, buffer_size, file) != nullptr; ++line_no) {
        char * first_id_str = strpbrk(buffer, DIGITS);
        char * second_id_str = first_id_str;

        const int32_t first_id = strtol(first_id_str, &second_id_str, 10);
        const int32_t second_id = strtol(second_id_str, nullptr, 10);

        first_ids[line_no] = first_id;
        second_ids[line_no] = second_id;
    }

    fclose(file);
}

int32_t part_one(char const * filename) {
    int32_t first_ids[1000] = {};
    int32_t second_ids[1000] = {};

    read_location_ids(filename, first_ids, second_ids);

    qsort(first_ids, 1000, sizeof(int32_t), compare_ints);
    qsort(second_ids, 1000, sizeof(int32_t), compare_ints);

    int32_t location_distance = 0;
    for (size_t i = 0; i < 1000; ++i) {
        location_distance += max(first_ids[i], second_ids[i]) - min(first_ids[i], second_ids[i]);
    }

    return location_distance;
}

int32_t part_two(char const * filename) {
    int32_t first_ids[1000] = {};
    int32_t second_ids[1000] = {};

    read_location_ids(filename, first_ids, second_ids);

    qsort(first_ids, 1000, sizeof(int32_t), compare_ints);
    qsort(second_ids, 1000, sizeof(int32_t), compare_ints);

    int32_t similarity_score = 0;
    for (size_t i = 0; i < 1000; ++i) {
        int32_t const first_value = first_ids[i];

        int32_t count = 0;
        for (size_t j = 0; j < 1000; ++j) {
            int32_t const second_value = second_ids[j];

            if (first_value == second_value) {
                ++count;
            }
        }

        similarity_score += first_value * count;
    }

    return similarity_score;
}
