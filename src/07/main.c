#include "lib/array.h"
#include "lib/file.h"
#include "lib/parse.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INPUT

#ifdef EXAMPLE
#define PART_ONE_FILENAME "example_1.txt"
#define PART_TWO_FILENAME "example_1.txt"
#define LINE_LENGTH 32
#elifdef INPUT
#define PART_ONE_FILENAME "input.txt"
#define PART_TWO_FILENAME "input.txt"
#define LINE_LENGTH 64
#endif

#define DIGITS "0123456789"

uint64_t part_one(char const * filename);
uint64_t part_two(char const * filename);

int main(void) {
    clock_t tic = clock();
    uint64_t part_one_solution = part_one(PART_ONE_FILENAME);
    clock_t toc = clock();
    double part_one_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    tic = clock();
    uint64_t part_two_solution = part_two(PART_ONE_FILENAME);
    toc = clock();
    double part_two_duration = (double)(toc - tic) / CLOCKS_PER_SEC * 1000.0;

    printf("Part One: %" PRIu64 ", calculated in %fms\n", part_one_solution, part_one_duration);
    printf("Part Two: %" PRIu64 ", calculated in %fms\n", part_two_solution, part_two_duration);

    return 0;
}

void array_sum(void * result, void const * value) {
    uint64_t result_ = *(uint64_t *)result;
    uint64_t value_ = *(uint64_t *)value;

    result_ += value_;

    memcpy(result, &result_, sizeof(uint64_t));
}

void array_product(void * result, void const * value) {
    uint64_t result_ = *(uint64_t *)result;
    uint64_t value_ = *(uint64_t *)value;

    result_ *= value_;

    memcpy(result, &result_, sizeof(uint64_t));
}

void parse_equations(char * string, uint64_t * target, array_t * values) {
    char * end_ptr = nullptr;
    *target = strtol(string, &end_ptr, 10);

    char * equation_values_ptr = strpbrk(end_ptr, DIGITS);
    parse_u64_numbers(equation_values_ptr, values);
}

bool equation_possible_part_one(array_t const * values, uint64_t target, uint64_t value, uint64_t * it) {
    if (it == array_t_end(values)) {
        return value == target;
    }

    if (value > target) {
        return false;
    }

    return equation_possible_part_one(values, target, value + *it, it + 1)
        || equation_possible_part_one(values, target, value * *it, it + 1);
}

uint64_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    uint64_t calibration_total = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        uint64_t target = {};
        array_t * values = array_t_new(sizeof(uint64_t));
        parse_equations(buffer, &target, values);

        uint64_t sum = 0;
        uint64_t product = 1;
        array_t_accumulate(values, &sum, array_sum);
        array_t_accumulate(values, &product, array_product);

        uint64_t * it = (uint64_t *)array_t_begin(values);
        uint64_t first_value = *it;
        if (equation_possible_part_one(values, target, first_value, ++it)) {
            calibration_total += target;
        }
    }

    fclose(file);

    return calibration_total;
}

uint64_t u64_concatenate(uint64_t first, uint64_t second) {
    uint64_t pow = 10;
    while (second >= pow) {
        pow *= 10;
    }
    return first * pow + second;
}

bool equation_possible_part_two(array_t const * values, uint64_t target, uint64_t value, uint64_t * it) {
    if (it == array_t_end(values)) {
        return value == target;
    }

    if (value > target) {
        return false;
    }

    return equation_possible_part_two(values, target, value + *it, it + 1)
        || equation_possible_part_two(values, target, value * *it, it + 1)
        || equation_possible_part_two(values, target, u64_concatenate(value, *it), it + 1);
}

uint64_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    uint64_t calibration_total = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        uint64_t target = {};
        array_t * values = array_t_new(sizeof(uint64_t));
        parse_equations(buffer, &target, values);

        uint64_t sum = 0;
        uint64_t product = 1;
        array_t_accumulate(values, &sum, array_sum);
        array_t_accumulate(values, &product, array_product);

        uint64_t * it = (uint64_t *)array_t_begin(values);
        uint64_t first_value = *it;
        if (equation_possible_part_two(values, target, first_value, ++it)) {
            calibration_total += target;
        }
    }

    fclose(file);

    return calibration_total;
}