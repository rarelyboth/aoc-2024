#include "lib/file.h"
#include "lib/array.h"

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
#define WORD_SEARCH_SIZE 10
#elifdef INPUT
#define FILENAME_1 "input.txt"
#define FILENAME_2 "input.txt"
#define LINE_LENGTH 128
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one(FILENAME_1));
    printf("Part Two: %" PRIi32 "\n", part_two(FILENAME_2));
    return 0;
}

typedef struct rule_t {
    int32_t first_page;
    int32_t second_page;
} rule_t;

void print_rules(array_t * rules) {
    for (rule_t * it = (rule_t *)array_t_begin(rules); it != array_t_end(rules); ++it) {
        printf("%d | %d\n", it->first_page, it->second_page);
    }
}

void print_update(array_t * update) {
    for (int32_t * it = (int32_t *)array_t_begin(update); it != array_t_end(update); ++it) {
        printf("%d, ", *it);
    }
}

void print_updates(array_t * updates) {
    for (array_t ** it = (array_t **)array_t_begin(updates); it != array_t_end(updates); ++it) {
        print_update(*it);
        printf("\n");
    }
}

rule_t parse_rule(char const * string) {
    char const * first_page_ptr = string;
    char * end_ptr = nullptr;
    int32_t first_page = strtol(first_page_ptr, &end_ptr, 10);

    char const * second_page_ptr = strchr(string, '|');
    int32_t second_page = strtol(second_page_ptr + 1, &end_ptr, 10);

    return (rule_t){ .first_page = first_page, .second_page = second_page };
}

array_t * parse_update(char const * string) {
    array_t * update = array_t_new(sizeof(int32_t));

    parse_numbers(string, update);

    return update;
}

bool int32_t_equals(void const * first, void const * second) {
    int32_t first_ = *(int32_t *)first;
    int32_t second_ = *(int32_t *)second;

    return first_ == second_;
}

bool update_satisfies_rules(array_t const * update, array_t const * rules) {
    for (rule_t * rule = (rule_t *)array_t_begin(rules); rule != array_t_end(rules); ++rule) {
        int32_t * first_page_ptr = (int32_t *)array_t_linear_find(update, &rule->first_page, int32_t_equals);
        if (first_page_ptr == array_t_end(update)) {
            continue;
        }

        int32_t * second_page_ptr = (int32_t *)array_t_linear_find(update, &rule->second_page, int32_t_equals);
        if (second_page_ptr == array_t_end(update)) {
            continue;
        }

        if (first_page_ptr > second_page_ptr) {
            return false;
        }
    }

    return true;
}

void sort_update_to_satisfy_rules(array_t const * update, array_t const * rules) {
    while (!update_satisfies_rules(update, rules)) {
        for (rule_t * rule = (rule_t *)array_t_begin(rules); rule != array_t_end(rules); ++rule) {
            int32_t * first_page_ptr = (int32_t *)array_t_linear_find(update, &rule->first_page, int32_t_equals);
            if (first_page_ptr == array_t_end(update)) {
                continue;
            }

            int32_t * second_page_ptr = (int32_t *)array_t_linear_find(update, &rule->second_page, int32_t_equals);
            if (second_page_ptr == array_t_end(update)) {
                continue;
            }

            if (first_page_ptr > second_page_ptr) {
                int32_t tmp = *first_page_ptr;
                *first_page_ptr = *second_page_ptr;
                *second_page_ptr = tmp;
                break;
            }
        }
    }
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    array_t * rules = array_t_new(sizeof(rule_t));
    {
        while (fgets(buffer, buffer_size, file) != nullptr) {
            if (*buffer == '\n') {
                break;
            }

            rule_t rule = parse_rule(buffer);
            array_t_append(rules, &rule);
        }
    }


    array_t * updates = array_t_new(sizeof(array_t *));
    {
        while (fgets(buffer, buffer_size, file) != nullptr) {
            array_t * update = parse_update(buffer);
            array_t_append(updates, &update);
        }
    }

    int32_t sum = 0;
    {
        for (array_t ** it = (array_t **)array_t_begin(updates); it != array_t_end(updates); ++it) {
            if (update_satisfies_rules(*it, rules)) {
                int32_t const middle_value = *(int32_t *)array_t_at(*it, array_t_length(*it) / 2);
                sum += middle_value;
            }
        }
    }

    array_t_free(rules);
    array_t_free(updates);

    return sum;
}

int32_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    array_t * rules = array_t_new(sizeof(rule_t));
    {
        while (fgets(buffer, buffer_size, file) != nullptr) {
            if (*buffer == '\n') {
                break;
            }

            rule_t rule = parse_rule(buffer);
            array_t_append(rules, &rule);
        }
    }


    array_t * updates = array_t_new(sizeof(array_t *));
    {
        while (fgets(buffer, buffer_size, file) != nullptr) {
            array_t * update = parse_update(buffer);
            array_t_append(updates, &update);
        }
    }

    int32_t sum = 0;
    {
        for (array_t ** it = (array_t **)array_t_begin(updates); it != array_t_end(updates); ++it) {
            if (!update_satisfies_rules(*it, rules)) {
                sort_update_to_satisfy_rules(*it, rules);

                int32_t const middle_value = *(int32_t *)array_t_at(*it, array_t_length(*it) / 2);
                sum += middle_value;
            }
        }
    }

    array_t_free(rules);
    array_t_free(updates);

    return sum;
}