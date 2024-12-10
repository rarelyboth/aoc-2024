#include "lib/file.h"
#include "lib/matrix.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT

#ifdef EXAMPLE
#define FILENAME_1 "example_1.txt"
#define FILENAME_2 "example_1.txt"
#define LINE_LENGTH 16
#define WORD_SEARCH_SIZE 10
#elifdef INPUT
#define FILENAME_1 "input.txt"
#define FILENAME_2 "input.txt"
#define WORD_SEARCH_SIZE 140
#define LINE_LENGTH 256
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

int32_t find_xmas_in_direction(matrix_t const * word_search, char last_char, size_t m, size_t n, size_t m_dir, size_t n_dir) {
    char current_char = *(char *)matrix_t_at(word_search, m, n);

    if (
        last_char == '!' && current_char == 'X'
        || last_char == 'X' && current_char == 'M'
        || last_char == 'M' && current_char == 'A'
    ) {
        size_t next_m = m + m_dir;
        size_t next_n = n + n_dir;

        if (next_m >= matrix_t_rows(word_search) || next_n >= matrix_t_columns(word_search)) {
            return 0;
        }

        return find_xmas_in_direction(word_search, current_char, next_m, next_n, m_dir, n_dir);
    }

    if (
        last_char == 'A' && current_char == 'S'
    ) {
        return 1;
    }

    return 0;
}

int32_t find_xmas_at(matrix_t const * word_search, size_t m, size_t n) {
    constexpr char last_char_sentinal = '!';

    return find_xmas_in_direction(word_search, last_char_sentinal, m, n, -1, 0)  // n
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, -1, 1)   // ne
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, 0, 1)    // e
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, 1, 1)    // se
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, 1, 0)    // s
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, 1, -1)   // sw
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, 0, -1)   // w
        + find_xmas_in_direction(word_search, last_char_sentinal, m, n, -1, -1); // nw
}

bool find_mas_in_direction(matrix_t const * word_search, size_t m, size_t n, bool direction) {
    char middle_char = *(char *)matrix_t_at(word_search, m, n);

    if (middle_char == 'A') {
        size_t dir = direction ? 1 : -1;
        char start_char = *(char *)matrix_t_at(word_search, m + (1 * dir), n - 1);
        char end_char = *(char *)matrix_t_at(word_search, m - (1 * dir), n + 1);

        if (
            (start_char == 'M' && end_char == 'S')
            || (start_char == 'S' && end_char == 'M')
        ) {
            return true;
        }
    }

    return false;
}

int32_t find_x_mas_at(matrix_t const * word_search, size_t m, size_t n) {
    if (
        find_mas_in_direction(word_search, m, n, true)
        && find_mas_in_direction(word_search, m, n, false)
    ) {
        return 1;
    }

    return 0;
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    matrix_t * word_search = matrix_t_new(WORD_SEARCH_SIZE, WORD_SEARCH_SIZE, sizeof(char));

    for (size_t m = 0; fgets(buffer, buffer_size, file) != nullptr; ++m) {
        char * letter_ptr = buffer;
        for (size_t n = 0; *letter_ptr != '\n'; ++letter_ptr, ++n) {
            matrix_t_set(word_search, m, n, letter_ptr);
        }
    }

    int32_t n_xmas = 0;
    for (size_t m = 0; m < matrix_t_rows(word_search); ++m) {
        for (size_t n = 0; n < matrix_t_columns(word_search); ++n) {
            n_xmas += find_xmas_at(word_search, m, n);
        }
    }

    return n_xmas;
}

int32_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    matrix_t * word_search = matrix_t_new(WORD_SEARCH_SIZE, WORD_SEARCH_SIZE, sizeof(char));

    for (size_t m = 0; fgets(buffer, buffer_size, file) != nullptr; ++m) {
        char * letter_ptr = buffer;
        for (size_t n = 0; *letter_ptr != '\n'; ++letter_ptr, ++n) {
            matrix_t_set(word_search, m, n, letter_ptr);
        }
    }

    int32_t n_xmas = 0;
    for (size_t m = 1; m < matrix_t_rows(word_search) - 1; ++m) {
        for (size_t n = 1; n < matrix_t_columns(word_search) - 1; ++n) {
            n_xmas += find_x_mas_at(word_search, m, n);
        }
    }

    return n_xmas;
}