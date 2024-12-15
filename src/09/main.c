#include "lib/array.h"
#include "lib/file.h"

#include <inttypes.h>
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#define INPUT

#ifdef EXAMPLE
#define PART_ONE_FILENAME "example_1.txt"
#define PART_TWO_FILENAME "example_1.txt"
#define LINE_LENGTH 32
#elifdef INPUT
#define PART_ONE_FILENAME "input.txt"
#define PART_TWO_FILENAME "input.txt"
#define LINE_LENGTH 32768
#endif

#define FREE_SPACE_SENTINAL -1

int64_t part_one(char const * filename);
int64_t part_two(char const * filename);

int main(void) {
    clock_t tic = clock();
    int64_t part_one_solution = part_one(PART_ONE_FILENAME);
    clock_t toc = clock();
    double part_one_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    tic = clock();
    int64_t part_two_solution = part_two(PART_ONE_FILENAME);
    toc = clock();
    double part_two_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    printf("Part One: %" PRIi64 ", calculated in %fms\n", part_one_solution, part_one_duration);
    printf("Part Two: %" PRIi64 ", calculated in %fms\n", part_two_solution, part_two_duration);

    return 0;
}

void print_file_system(array_t const * file_system) {
    for (int32_t * it = (int32_t *)array_t_begin(file_system); it != array_t_end(file_system); ++it) {
        if (*it == -1) {
            printf(".");
        } else {
            printf("%" PRIi32, *it);
        }
    }
    printf("\n");
}

void rearrange_file_system_part_one(array_t * file_system) {
    int32_t * f_it = (int32_t *)array_t_begin(file_system);
    int32_t * r_it = (int32_t *)array_t_rbegin(file_system);
    for (; f_it < r_it; ++f_it) {
        if (*f_it == FREE_SPACE_SENTINAL) {
            for (; f_it < r_it; --r_it) {
                if (*r_it != FREE_SPACE_SENTINAL) {
                    int32_t tmp = *f_it;
                    *f_it = *r_it;
                    *r_it = tmp;
                    break;
                }
            }

            r_it = (int32_t *)array_t_rbegin(file_system);
        }
    }
}

int64_t calculate_file_system_checksum(array_t const * file_system) {
    int64_t checksum = 0;

    int64_t i = 0;
    for (
        int32_t * it = array_t_begin(file_system)
        ; it != array_t_end(file_system)
        ; ++it, ++i
    ) {
        if (*it != FREE_SPACE_SENTINAL) {
            checksum += *it * i;
        }
    }

    return checksum;
}

int64_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t const free_space_sentinal = FREE_SPACE_SENTINAL;
    int32_t file_id = 0;
    bool is_file = true;
    array_t * file_system = array_t_new(sizeof(int32_t));
    while (fgets(buffer, buffer_size, file) != nullptr) {
        for (char * it = buffer; *it != '\n'; ++it) {
            int32_t blocks = *it - '0';

            if (is_file) {
                for (int32_t i = 0; i < blocks; ++i) {
                    array_t_append(file_system, &file_id);
                }
                ++file_id;
            } else {
                for (int32_t i = 0; i < blocks; ++i) {
                    array_t_append(file_system, &free_space_sentinal);
                }
            }

            is_file = !is_file;
        }
    }
    fclose(file);

    rearrange_file_system_part_one(file_system);
    int64_t checksum = calculate_file_system_checksum(file_system);

    array_t_free(file_system);

    return checksum;
}

void rearrange_file_system_part_two(array_t * file_system) {
    int32_t * f_it = (int32_t *)array_t_begin(file_system);
    int32_t * r_it = (int32_t *)array_t_rbegin(file_system);

    for (; r_it > f_it; --r_it) {
        int32_t file_id = *r_it;
        if (*r_it != FREE_SPACE_SENTINAL) {
            int32_t * file_it = r_it;
            for (; *file_it == file_id; --file_it) {
            }

            for (; r_it > f_it; ++f_it) {
                if (*f_it == FREE_SPACE_SENTINAL) {
                    int32_t * free_it = f_it;
                    for (; *free_it == FREE_SPACE_SENTINAL; ++free_it) {
                    }

                    ptrdiff_t free_space = free_it - f_it;
                    ptrdiff_t file_size = r_it - file_it;
                    if (free_space >= file_size) {
                        for (size_t i = 0; i < file_size; ++i) {
                            int32_t tmp = *(f_it + i);
                            *(f_it + i) = *(r_it - i);
                            *(r_it - i) = tmp;
                        }

                        r_it = file_it + 1;
                        break;
                    }

                    f_it = free_it - 1;
                }
            }

            r_it = file_it + 1;
        }

        f_it = (int32_t *)array_t_begin(file_system);
    }
}

int64_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t const free_space_sentinal = FREE_SPACE_SENTINAL;
    int32_t file_id = 0;
    bool is_file = true;
    array_t * file_system = array_t_new(sizeof(int32_t));
    while (fgets(buffer, buffer_size, file) != nullptr) {
        for (char * it = buffer; *it != '\n'; ++it) {
            int32_t blocks = *it - '0';

            if (is_file) {
                for (int32_t i = 0; i < blocks; ++i) {
                    array_t_append(file_system, &file_id);
                }
                ++file_id;
            } else {
                for (int32_t i = 0; i < blocks; ++i) {
                    array_t_append(file_system, &free_space_sentinal);
                }
            }

            is_file = !is_file;
        }
    }
    fclose(file);

    rearrange_file_system_part_two(file_system);
    int64_t checksum = calculate_file_system_checksum(file_system);

    array_t_free(file_system);

    return checksum;
}