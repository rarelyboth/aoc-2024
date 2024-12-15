#include "lib/array.h"
#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#define INPUT

#ifdef EXAMPLE
#define PART_ONE_FILENAME "example_1.txt"
#define PART_TWO_FILENAME "example_1.txt"
#define LINE_LENGTH 16
#define MAP_SIZE 12
#elifdef INPUT
#define PART_ONE_FILENAME "input.txt"
#define PART_TWO_FILENAME "input.txt"
#define LINE_LENGTH 64
#define MAP_SIZE 50
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    clock_t tic = clock();
    int32_t part_one_solution = part_one(PART_ONE_FILENAME);
    clock_t toc = clock();
    double part_one_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    tic = clock();
    int32_t part_two_solution = part_two(PART_ONE_FILENAME);
    toc = clock();
    double part_two_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    printf("Part One: %" PRIi32 ", calculated in %fms\n", part_one_solution, part_one_duration);
    printf("Part Two: %" PRIi32 ", calculated in %fms\n", part_two_solution, part_two_duration);

    return 0;
}

typedef struct vec2_t {
    int32_t x;
    int32_t y;
} vec2_t;

bool vec2_t_equality(void const * first, void const * second) {
    vec2_t first_ = *(vec2_t *)first;
    vec2_t second_ = *(vec2_t *)second;

    return first_.x == second_.x && first_.y == second_.y;
}

typedef struct antenna_t {
    vec2_t position;
    char frequency;
} antenna_t;

int antenna_t_compare(void const * first, void const * second) {
    antenna_t first_ = *(antenna_t *)first;
    antenna_t second_ = *(antenna_t *)second;

    if (first_.frequency == second_.frequency) {
        return 0;
    }

    return first_.frequency < second_.frequency ? -1 : 1;
}

bool anti_node_within_map(vec2_t anti_node) {
    return anti_node.x >= 0 && anti_node.x < MAP_SIZE
        && anti_node.y >= 0 && anti_node.y < MAP_SIZE;
}

void record_anti_node(vec2_t anti_node, array_t * anti_nodes) {
    bool const unseen = array_t_linear_find(anti_nodes, &anti_node, vec2_t_equality) == array_t_end(anti_nodes);

    if (anti_node_within_map(anti_node) && unseen) {
        array_t_append(anti_nodes, &anti_node);
    }
}

void locate_unique_anti_nodes_part_one(array_t const * antennae, array_t * anti_nodes) {
    for (antenna_t * first_it = array_t_begin(antennae); first_it != array_t_end(antennae); ++first_it) {
        for (antenna_t * second_it = array_t_begin(antennae); second_it != array_t_end(antennae); ++second_it) {
            if (first_it != second_it && first_it->frequency == second_it->frequency) {
                vec2_t antennae_direction = {
                    .x = first_it->position.x - second_it->position.x,
                    .y = first_it->position.y - second_it->position.y
                };

                vec2_t first_anti_node = {
                    .x = first_it->position.x + antennae_direction.x,
                    .y = first_it->position.y + antennae_direction.y
                };
                vec2_t second_anti_node = {
                    .x = second_it->position.x - antennae_direction.x,
                    .y = second_it->position.y - antennae_direction.y
                };

                record_anti_node(first_anti_node, anti_nodes);
                record_anti_node(second_anti_node, anti_nodes);
            }
        }
    }
}

void print_antennae(array_t * antennae) {
    for (antenna_t * it = (antenna_t *)array_t_begin(antennae); it != array_t_end(antennae); ++it) {
        printf("%c, %" PRIi32 ", %" PRIi32 "\n", it->frequency, it->position.x, it->position.y);
    }
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    array_t * antennae = array_t_new(sizeof(antenna_t));
    {
        for (size_t y = 0; fgets(buffer, buffer_size, file) != nullptr; ++y) {
            char * it = buffer;
            for (size_t x = 0; *it != '\n'; ++x, ++it) {
                if (*it != '.') {
                    antenna_t antenna = {.frequency = *it, .position = {.x = x, .y = y}};
                    array_t_append(antennae, &antenna);
                }
            }
        }
        fclose(file);

    }

    array_t * anti_nodes = array_t_new(sizeof(vec2_t));
    {
        locate_unique_anti_nodes_part_one(antennae, anti_nodes);
    }

    int32_t unique_anti_nodes = (int32_t)array_t_length(anti_nodes);

    array_t_free(antennae);
    array_t_free(anti_nodes);

    return unique_anti_nodes;
}

void locate_unique_anti_nodes_part_two(array_t const * antennae, array_t * anti_nodes) {
    for (antenna_t * first_it = array_t_begin(antennae); first_it != array_t_end(antennae); ++first_it) {
        for (antenna_t * second_it = array_t_begin(antennae); second_it != array_t_end(antennae); ++second_it) {
            if (first_it != second_it && first_it->frequency == second_it->frequency) {
                vec2_t antennae_direction = {
                    .x = first_it->position.x - second_it->position.x,
                    .y = first_it->position.y - second_it->position.y
                };

                vec2_t first_anti_node = first_it->position;
                while (anti_node_within_map(first_anti_node)) {
                    record_anti_node(first_anti_node, anti_nodes);
                    first_anti_node = (vec2_t){
                        .x = first_anti_node.x + antennae_direction.x,
                        .y = first_anti_node.y + antennae_direction.y
                    };
                }

                vec2_t second_anti_node = second_it->position;
                while (anti_node_within_map(second_anti_node)) {
                    record_anti_node(second_anti_node, anti_nodes);
                    second_anti_node = (vec2_t){
                        .x = second_anti_node.x - antennae_direction.x,
                        .y = second_anti_node.y - antennae_direction.y
                    };
                }

                record_anti_node(first_anti_node, anti_nodes);
                record_anti_node(second_anti_node, anti_nodes);
            }
        }
    }
}

int32_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    array_t * antennae = array_t_new(sizeof(antenna_t));
    {
        for (size_t y = 0; fgets(buffer, buffer_size, file) != nullptr; ++y) {
            char * it = buffer;
            for (size_t x = 0; *it != '\n'; ++x, ++it) {
                if (*it != '.') {
                    antenna_t antenna = {.frequency = *it, .position = {.x = x, .y = y}};
                    array_t_append(antennae, &antenna);
                }
            }
        }

        fclose(file);
    }

    array_t * anti_nodes = array_t_new(sizeof(vec2_t));
    {
        locate_unique_anti_nodes_part_two(antennae, anti_nodes);
    }

    int32_t unique_anti_nodes = (int32_t)array_t_length(anti_nodes);

    array_t_free(antennae);
    array_t_free(anti_nodes);

    return unique_anti_nodes;
}