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

bool vec2_t_equals(void const * first, void const * second) {
    vec2_t first_ = *(vec2_t *)first;
    vec2_t second_ = *(vec2_t *)second;

    return first_.x == second_.x && first_.y == second_.y;
}

void visit_location(matrix_t const * map, array_t * visited, vec2_t location, vec2_t direction) {
    if (array_t_linear_find(visited, &location, vec2_t_equals) == array_t_end(visited)) {
        array_t_append(visited, &location);
    }

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
    while (next_location_char == '#') {
        direction = (vec2_t){ .x = -direction.y, .y = direction.x };
        next_location = (vec2_t){ location.x + direction.x, location.y + direction.y };
        next_location_char = *(char *)matrix_t_at(map, next_location.y, next_location.x);
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

    array_t * visited = array_t_new(sizeof(vec2_t));
    visit_location(map, visited, guard_starting_location, (vec2_t){ .x = 0, .y = -1 });
    int32_t n_visited = (int32_t)array_t_length(visited);

    matrix_t_free(map);
    array_t_free(visited);

    return n_visited;
}

typedef struct visit_t {
    vec2_t position;
    vec2_t direction;
} visit_t;

bool visit_t_equals(void const * first, void const * second) {
    visit_t * first_ = (visit_t *)first;
    visit_t * second_ = (visit_t *)second;

    return first_->position.x == second_->position.x
        && first_->position.y == second_->position.y
        && first_->direction.x == second_->direction.x
        && first_->direction.y == second_->direction.y;
}

bool visit_location_detect_cycles(matrix_t const * map, matrix_t * visited, vec2_t position, vec2_t direction) {
    visit_t current_visit = { .position = position, .direction = direction };

    array_t * visits_at_location = *(array_t **)matrix_t_at(visited, position.y, position.x);
    if (array_t_linear_find(visits_at_location, &current_visit, visit_t_equals) != array_t_end(visits_at_location)) {
        return true;
    }
    array_t_append(visits_at_location, &current_visit);

    vec2_t next_location = { position.x + direction.x, position.y + direction.y };
    if (
        next_location.x < 0
        || next_location.y < 0
        || next_location.x >= matrix_t_columns(map)
        || next_location.y >= matrix_t_rows(map)
    ) {
        return false;
    }

    char next_location_char = *(char *)matrix_t_at(map, next_location.y, next_location.x);
    while (next_location_char == '#') {
        direction = (vec2_t){ .x = -direction.y, .y = direction.x };
        next_location = (vec2_t){ position.x + direction.x, position.y + direction.y };
        next_location_char = *(char *)matrix_t_at(map, next_location.y, next_location.x);
    }

    visit_location_detect_cycles(map, visited, next_location, direction);
}

int32_t part_two(char const * filename) {
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

    array_t * guard_path = array_t_new(sizeof(vec2_t));
    visit_location(map, guard_path, guard_starting_location, (vec2_t){ .x = 0, .y = -1 });

    int32_t n_cyclic_obstacles = 0;
    {
        for (vec2_t * it = (vec2_t *)array_t_begin(guard_path); it != array_t_end(guard_path); ++it) {
            matrix_t * visited = matrix_t_new(MAP_SIZE, MAP_SIZE, sizeof(array_t *));
            for (size_t m = 0; m < matrix_t_rows(visited); ++m) {
                for (size_t n = 0; n < matrix_t_columns(visited); ++n) {
                    array_t * tmp = array_t_new(sizeof(visit_t));
                    matrix_t_set(visited, m, n, &tmp);
                }
            }

            char obstacle = '#';
            matrix_t_set(map, it->y, it->x, &obstacle);

            if (visit_location_detect_cycles(map, visited, guard_starting_location, (vec2_t){ .x = 0, .y = -1 })) {
                ++n_cyclic_obstacles;
            }

            char space = '.';
            matrix_t_set(map, it->y, it->x, &space);

            for (size_t m = 0; m < matrix_t_rows(visited); ++m) {
                for (size_t n = 0; n < matrix_t_columns(visited); ++n) {
                    array_t * visit = *(array_t **)matrix_t_at(visited, m, n);
                    array_t_free(visit);
                }
            }

            matrix_t_free(visited);
        }
    }

    matrix_t_free(map);

    return n_cyclic_obstacles;
}
