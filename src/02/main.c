#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <lib/array.h>
#include <lib/parse.h>

#define EXAMPLE

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

bool report_levels_safe(array_t const * array) {

}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    size_t constexpr buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t safe_reports = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        char * start_ptr = buffer;
        char * end_ptr = buffer;

        bool first = true;
        bool report_safe = true;
        int32_t last_difference = 0;
        while (true) {
            int32_t first_level = strtol(start_ptr, &start_ptr, 10);
            int32_t second_level = strtol(start_ptr, &end_ptr, 10);
            if (end_ptr == start_ptr) {
                break;
            }

            int32_t difference = first_level - second_level;

            if (difference == 0 || abs(difference) > 3) {
                report_safe = false;
                break;
            }

            if (first) {
                last_difference = difference;
                first = false;
                continue;
            }

            if (difference < 0 != last_difference < 0) {
                report_safe = false;
                break;
            }
        }

        if (report_safe) {
            ++safe_reports;
        }
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

        bool first = true;
        bool report_safe = true;
        int32_t last_difference = 0;
        for (int32_t * it = (int32_t *)array_t_begin(report_levels)
            , * next_it = it + 1
            ; it != array_t_end(report_levels)
            ; ++it
            , ++next_it) {
            int32_t difference = *it - *next_it;

            if (difference == 0 || abs(difference) > 3) {
                report_safe = false;
                break;
            }

            if (first) {
                last_difference = difference;
                first = false;
                continue;
            }

            if (difference < 0 != last_difference < 0) {
                report_safe = false;
                break;
            }
        }

        if (report_safe) {
            ++safe_reports;
        }

        array_t_free(report_levels);
    }

    fclose(file);

    return safe_reports;
}
