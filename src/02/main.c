#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <lib/array.h>
#include <lib/parse.h>

#define INPUT

#ifdef EXAMPLE
#define FILENAME "example.txt"
#define LINE_LENGTH 16
#elifdef INPUT
#define FILENAME "input.txt"
#define LINE_LENGTH 32
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one(FILENAME));
    printf("Part Two: %" PRIi32 "\n", part_two(FILENAME));
    return 0;
}

array_t * copy_without(array_t const * array, size_t index) {
    array_t * copy = array_t_copy(array);
    array_t_remove(copy, index);

    return copy;
}

void print_array(array_t const * array) {
    for (int32_t * it = (int32_t *)array_t_begin(array); it != array_t_end(array); ++it) {
        printf("%d ", *it);
    }
    printf("\n");
}

bool report_levels_safe(array_t const * array, bool allow_bad_level) {
    bool report_safe = true;

    int32_t * it = (int32_t *)array_t_begin(array);
    int32_t * next_it = it + 1;

    int32_t first_difference = *it - *next_it;
    for (; next_it != array_t_end(array); ++it, ++next_it) {
        int32_t difference = *it - *next_it;

        if (abs(difference) < 1 || abs(difference) > 3 || difference < 0 != first_difference < 0) {
            report_safe = false;
            break;
        }
    }

    if (allow_bad_level && !report_safe) {
        for (int32_t * it = (int32_t *)array_t_begin(array); it != array_t_end(array); ++it) {
            array_t * without_it = copy_without(array, it - (int32_t *)array_t_begin(array));

            report_safe = report_levels_safe(without_it, false);

            if (report_safe) {
                break;
            }

            array_t_free(without_it);
        }
    }

    return report_safe;
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    size_t constexpr buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t safe_reports = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        array_t * report_levels = array_t_new(sizeof(int32_t));
        parse_numbers(buffer, report_levels);

        if (report_levels_safe(report_levels, false)) {
            ++safe_reports;
        }

        array_t_free(report_levels);
    }

    fclose(file);

    return safe_reports;
}

int32_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    size_t constexpr buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t safe_reports = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        array_t * report_levels = array_t_new(sizeof(int32_t));
        parse_numbers(buffer, report_levels);

        if (report_levels_safe(report_levels, true)) {
            ++safe_reports;
        }

        array_t_free(report_levels);
    }

    fclose(file);

    return safe_reports;
}
