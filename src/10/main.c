#include "lib/array.h"
#include "lib/file.h"
#include "lib/graph.h"
#include "lib/matrix.h"

#include <inttypes.h>
#include <stdio.h>
#include <time.h>

#define INPUT

#ifdef EXAMPLE
#define PART_ONE_FILENAME "example_1.txt"
#define PART_TWO_FILENAME "example_1.txt"
#define LINE_LENGTH 16
#define MAP_SIZE 8
#elifdef INPUT
#define PART_ONE_FILENAME "input.txt"
#define PART_TWO_FILENAME "input.txt"
#define LINE_LENGTH 64
#define MAP_SIZE 50
#endif

#define TRAILHEAD_ELEVATION 0
#define REQUIRED_GRADIENT 1
#define MAX_ELEVATION 9

int32_t part_one(char const * filename);
int64_t part_two(char const * filename);

int main(void) {
    clock_t tic = clock();
    int32_t part_one_solution = part_one(PART_ONE_FILENAME);
    clock_t toc = clock();
    double part_one_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    tic = clock();
    int64_t part_two_solution = part_two(PART_ONE_FILENAME);
    toc = clock();
    double part_two_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    printf("Part One: %" PRIi32 ", calculated in %fms\n", part_one_solution, part_one_duration);
    printf("Part Two: %" PRIi64 ", calculated in %fms\n", part_two_solution, part_two_duration);

    return 0;
}

void print_map(matrix_t * map) {
    for (size_t m = 0; m < matrix_t_rows(map); ++m) {
        for (size_t n = 0; n < matrix_t_columns(map); ++n) {
            vertex_t * vertex = *(vertex_t **)matrix_t_at(map, m, n);
            int32_t elevation = *(int32_t *)vertex_t_value(vertex);

            printf("%" PRIi32, elevation);
        }
        printf("\n");
    }
}

void add_edges_between_map_locations(graph_t * graph, vertex_t const * from_vertex, vertex_t const * to_vertex) {
    int32_t const from_elevation = *(int32_t *)vertex_t_value(from_vertex);
    int32_t const to_elevation = *(int32_t *)vertex_t_value(to_vertex);

    int32_t const gradient_from = from_elevation - to_elevation;
    int32_t const gradient_to = to_elevation - from_elevation;

    edge_t_new(graph, from_vertex, to_vertex, &gradient_to);
    edge_t_new(graph, to_vertex, from_vertex, &gradient_from);
}

void build_trails(graph_t * trails, matrix_t * map) {
    for (size_t m = 0; m < matrix_t_rows(map); ++m) {
        for (size_t n = 0; n < matrix_t_columns(map) - 1; ++n) {
            vertex_t * current = *(vertex_t **)matrix_t_at(map, m, n);
            vertex_t * next = *(vertex_t **)matrix_t_at(map, m, n + 1);

            add_edges_between_map_locations(trails, current, next);
        }
    }

    for (size_t n = 0; n < matrix_t_columns(map); ++n) {
        for (size_t m = 0; m < matrix_t_rows(map) - 1; ++m) {
            vertex_t * current = *(vertex_t **)matrix_t_at(map, m, n);
            vertex_t * next = *(vertex_t **)matrix_t_at(map, m + 1, n);

            add_edges_between_map_locations(trails, current, next);
        }
    }
}

int32_t count_trails(graph_t const * trails, vertex_t const * position) {
    int32_t elevation = *(int32_t *)vertex_t_value(position);
    if (elevation == MAX_ELEVATION) {
        return 1;
    }

    int32_t trails_from_position = 0;
    array_t const * edges_from = *(array_t **)graph_t_get_edges_from(trails, position);
    for (void const * it = array_t_begin(edges_from); it != array_t_end(edges_from); it += edge_t_size(trails)) {
        int32_t gradient_to = *(int32_t *)edge_t_value(it);
        if (gradient_to == REQUIRED_GRADIENT) {
            vertex_t const * edge_to = edge_t_to(it);
            trails_from_position += count_trails(trails, edge_to);
        }
    }

    return trails_from_position;
}

void count_unique_peaks(graph_t const * trails, vertex_t const * position, array_t * peaks) {
    int32_t elevation = *(int32_t *)vertex_t_value(position);
    if (elevation == MAX_ELEVATION) {
        if (array_t_linear_find(peaks, position, vertex_t_equality) == array_t_end(peaks)) {
            array_t_append(peaks, position);
        }

        return;
    }

    array_t const * edges_from = *(array_t **)graph_t_get_edges_from(trails, position);
    for (void const * it = array_t_begin(edges_from); it != array_t_end(edges_from); it += edge_t_size(trails)) {
        int32_t gradient_to = *(int32_t *)edge_t_value(it);
        if (gradient_to == REQUIRED_GRADIENT) {
            vertex_t const * edge_to = edge_t_to(it);
            count_unique_peaks(trails, edge_to, peaks);
        }
    }
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    graph_t * trails = graph_t_new(sizeof(int32_t), sizeof(int32_t));
    matrix_t * map = matrix_t_new(MAP_SIZE, MAP_SIZE, sizeof(vertex_t *));
    for (size_t m = 0; fgets(buffer, buffer_size, file) != nullptr; ++m) {
        char * it = buffer;
        for (size_t n = 0; *it != '\n'; ++it, ++n) {
            int32_t elevation = *it - '0';
            vertex_t * location = vertex_t_new(trails, &elevation);
            matrix_t_set(map, m, n, &location);
        }
    }
    fclose(file);

    build_trails(trails, map);

    print_map(map);

    int32_t trailhead_total = 0;
    array_t * vertices = graph_t_get_vertices(trails);
    for (void * it = array_t_begin(vertices); it != array_t_end(vertices); it += vertex_t_size(trails)) {
        int32_t elevation = *(int32_t *)vertex_t_value(it);
        if (elevation == TRAILHEAD_ELEVATION) {
            array_t * peaks = array_t_new(vertex_t_size(trails));
            count_unique_peaks(trails, it, peaks);
            trailhead_total += (int32_t)array_t_length(peaks);
        }
    }



    matrix_t_free(map);

    return trailhead_total;
}

int64_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    graph_t * trails = graph_t_new(sizeof(int32_t), sizeof(int32_t));
    matrix_t * map = matrix_t_new(MAP_SIZE, MAP_SIZE, sizeof(vertex_t *));
    for (size_t m = 0; fgets(buffer, buffer_size, file) != nullptr; ++m) {
        char * it = buffer;
        for (size_t n = 0; *it != '\n'; ++it, ++n) {
            int32_t elevation = *it - '0';
            vertex_t * location = vertex_t_new(trails, &elevation);
            matrix_t_set(map, m, n, &location);
        }
    }
    fclose(file);

    build_trails(trails, map);

    print_map(map);

    int32_t trailhead_total = 0;
    array_t * vertices = graph_t_get_vertices(trails);
    for (void * it = array_t_begin(vertices); it != array_t_end(vertices); it += vertex_t_size(trails)) {
        int32_t elevation = *(int32_t *)vertex_t_value(it);
        if (elevation == TRAILHEAD_ELEVATION) {
            trailhead_total += count_trails(trails, it);
        }
    }

    matrix_t_free(map);

    return trailhead_total;
}