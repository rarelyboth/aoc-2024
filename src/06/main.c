#include "lib/file.h"
#include "lib/matrix.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/parse.h>

#define INPUT

#ifdef EXAMPLE
#define FILENAME_1 "example_1.txt"
#define FILENAME_2 "example_1.txt"
#define LINE_LENGTH 16
#define MAP_SIZE 10
#elifdef INPUT
#define FILENAME_1 "input.txt"
#define FILENAME_2 "input.txt"
#define LINE_LENGTH 256
#define MAP_SIZE 130
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one(FILENAME_1));
    printf("Part Two: %" PRIi32 "\n", part_two(FILENAME_2));
    return 0;
}

void print_char_matrix(matrix_t const * matrix) {
    for (size_t m = 0; m < matrix_t_rows(matrix); ++m) {
        for (size_t n = 0; n < matrix_t_columns(matrix); ++n) {
            char character = *(char *)matrix_t_at(matrix, m, n);
            printf("%c ", character);
        }
        printf("\n");
    }
}

typedef struct vec2_t {
    int32_t x;
    int32_t y;
} vec2_t;

void visit_location(matrix_t const * map, matrix_t * visited, vec2_t location, vec2_t direction) {
    bool visit_status = true;
    matrix_t_set(visited, location.y, location.x, &visit_status);

    vec2_t next_location = { location.x + direction.x, location.y + direction.y };

    if (
        next_location.x < 0
        || next_location.y < 0
        || next_location.x >= matrix_t_columns(map)
        || next_location.y >= matrix_t_rows(map)
    ) {
        return;
    }

    char next_location_char = *(char *)matrix_t_at(map, next_location.y, next_location.x);
    if (next_location_char == '#') {
        direction = (vec2_t){ .x = -direction.y, .y = direction.x };
        next_location = (vec2_t){ location.x + direction.x, location.y + direction.y };
    }

    visit_location(map, visited, next_location, direction);
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    vec2_t guard_starting_location = {};
    matrix_t * map = matrix_t_new(MAP_SIZE, MAP_SIZE, sizeof(char));
    {
        for (size_t m = 0; fgets(buffer, buffer_size, file) != nullptr; ++m) {
            char * it = buffer;
            for (size_t n = 0; *it != '\n'; ++it, ++n) {
                matrix_t_set(map, m, n, it);

                if (*it == '^') {
                    guard_starting_location = (vec2_t){ .x = n, .y = m };
                }
            }
        }
    }

    matrix_t * visited = matrix_t_new(MAP_SIZE, MAP_SIZE, sizeof(bool));
    {
        visit_location(map, visited, guard_starting_location, (vec2_t){ .x = 0, .y = -1 });
    }

    int32_t n_visited = 0;
    {
        for (size_t m = 0; m < matrix_t_rows(visited); ++m) {
            for (size_t n = 0; n < matrix_t_columns(visited); ++n) {
                bool was_visited = *(bool *)matrix_t_at(visited, m, n);

                if (was_visited) {
                    ++n_visited;
                }
            }
        }
    }

    matrix_t_free(map);
    matrix_t_free(visited);

    return n_visited;
}

int32_t part_two(char const * filename) {
    return 0;
}
