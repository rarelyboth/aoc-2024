#include "lib/array.h"
#include "lib/file.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT

#ifdef EXAMPLE
#define FILENAME_1 "example_1.txt"
#define FILENAME_2 "example_2.txt"
#define LINE_LENGTH 128
#elifdef INPUT
#define FILENAME_1 "input.txt"
#define FILENAME_2 "input.txt"
#define LINE_LENGTH 18000
#endif

int32_t part_one(char const * filename);
int32_t part_two(char const * filename);

int main(void) {
    printf("Part One: %" PRIi32 "\n", part_one(FILENAME_1));
    printf("Part Two: %" PRIi32 "\n", part_two(FILENAME_2));
    return 0;
}

typedef enum fn_e {
    DO = 0,
    DONT,
    MUL
} fn_e;

typedef struct mul_params {
    int32_t first;
    int32_t second;
} mul_params;

typedef struct void_params {
} void_params;

typedef struct fn_call_t {
    fn_e function;
    union {
        mul_params mul_params;
        void_params void_params;
    } parameters;
} fn_call_t;

fn_call_t * parse_next_do_fn(char * string, char ** end_ptr) {
    char const * do_fn_name = "do()";

    char * next_do = strstr(string, do_fn_name);
    if (next_do != nullptr) {
        fn_call_t * do_fn = malloc(sizeof(fn_call_t));
        do_fn->function = DO;
        do_fn->parameters.void_params = (void_params){};

        *end_ptr = next_do + strlen(do_fn_name);
        return do_fn;
    }

    *end_ptr = string;
    return nullptr;
}

fn_call_t * parse_next_dont_fn(char * string, char ** end_ptr) {
    char const * dont_fn_name = "don't()";

    char * next_dont = strstr(string, dont_fn_name);
    if (next_dont != nullptr) {
        fn_call_t * dont_fn = malloc(sizeof(fn_call_t));
        dont_fn->function = DONT;
        dont_fn->parameters.void_params = (void_params){};

        *end_ptr = next_dont + strlen(dont_fn_name);
        return dont_fn;
    }

    *end_ptr = string;
    return nullptr;
}

fn_call_t * parse_next_mul_fn(char * string, char ** end_ptr) {
    char * arg_1_ptr = strstr(string, "mul(") + 4;
    if (arg_1_ptr - 4 == NULL) {
        *end_ptr = string;
        return nullptr;
    }

    char * arg_2_ptr = strchr(arg_1_ptr, ',') + 1;
    if (arg_2_ptr - 1 == nullptr) {
        *end_ptr = string;
        return nullptr;
    }

    char * cmd_end_ptr = strchr(arg_2_ptr, ')');
    if (cmd_end_ptr == nullptr) {
        *end_ptr = string;
        return nullptr;
    }

    char * end = arg_1_ptr;
    int32_t arg_1 = strtol(arg_1_ptr, &end, 10);
    if (end != arg_2_ptr - 1) {
        *end_ptr = arg_1_ptr;
        return nullptr;
    }

    int32_t arg_2 = strtol(arg_2_ptr, &end, 10);
    if (end != cmd_end_ptr) {
        *end_ptr = arg_1_ptr;
        return nullptr;
    }

    fn_call_t * mul_fn = malloc(sizeof(fn_call_t));
    mul_fn->function = MUL;
    mul_fn->parameters.mul_params = (mul_params){ .first = arg_1, .second = arg_2 };

    *end_ptr = cmd_end_ptr + 1;
    return mul_fn;
}

int compare_char_ptr(void const * first, void const * second) {
    char const ** first_ = (char const **)first;
    char const ** second_ = (char const **)second;

    if (*first_ == *second_) {
        return 0;
    }

    return *first_ < *second_ ? -1 : 1;
}

fn_call_t * parse_next_fn(char * string, char ** end_ptr) {
    char * do_end = string;
    fn_call_t * next_do = parse_next_do_fn(string, &do_end);

    char * dont_end = string;
    fn_call_t * next_dont = parse_next_dont_fn(string, &dont_end);

    char * mul_end = string;
    fn_call_t * next_mul = parse_next_mul_fn(string, &mul_end);

    if (!(next_do || next_dont || next_mul)) {
        *end_ptr = mul_end;
        return nullptr;
    }

    array_t * fn_calls = array_t_new(sizeof(char **));
    array_t_append(fn_calls, &do_end);
    array_t_append(fn_calls, &dont_end);
    array_t_append(fn_calls, &mul_end);
    array_t_sort(fn_calls, compare_char_ptr);

    char * earliest_fn = string;
    for (char ** it = (char **)array_t_begin(fn_calls); it != array_t_end(fn_calls); ++it) {
        if (*it != string) {
            earliest_fn = *it;
            break;
        }
    }

    if (earliest_fn == do_end) {
        free(next_dont);
        free(next_mul);
        *end_ptr = do_end;
        return next_do;
    }

    if (earliest_fn == dont_end) {
        free(next_do);
        free(next_mul);
        *end_ptr = dont_end;
        return next_dont;
    }

    if (earliest_fn == mul_end) {
        free(next_do);
        free(next_dont);
        *end_ptr = mul_end;
        return next_mul;
    }

    array_t_free(fn_calls);
}

int32_t part_one(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    int32_t sum = 0;
    while (fgets(buffer, buffer_size, file) != nullptr) {
        char * start_ptr = buffer;
        char * end_ptr = start_ptr;
        while (true) {
            fn_call_t * mul = parse_next_mul_fn(start_ptr, &end_ptr);
            if (end_ptr == start_ptr) {
                break;
            }
            if (mul == nullptr) {
                start_ptr = end_ptr + 1;
                continue;
            }

            sum += mul->parameters.mul_params.first * mul->parameters.mul_params.second;
            free(mul);

            start_ptr = end_ptr;
        }
    }

    return sum;
}

int32_t part_two(char const * filename) {
    FILE * file = file_open(filename, "r");

    constexpr size_t buffer_size = LINE_LENGTH;
    char buffer[buffer_size] = {};

    array_t * fn_calls = array_t_new(sizeof(fn_call_t));
    while (fgets(buffer, buffer_size, file) != nullptr) {
        char * start_ptr = buffer;
        char * end_ptr = start_ptr;
        while (true) {
            fn_call_t * fn = parse_next_fn(start_ptr, &end_ptr);
            if (end_ptr == start_ptr) {
                break;
            }
            if (fn == nullptr) {
                start_ptr = end_ptr;
                continue;
            }

            array_t_append(fn_calls, fn);

            start_ptr = end_ptr;
        }
    }

    size_t n_do = 0;
    size_t n_dont = 0;
    size_t n_mul = 0;
    for (fn_call_t * it = (fn_call_t *)array_t_begin(fn_calls); it != array_t_end(fn_calls); ++it) {
        if (it->function == DO) {
            ++n_do;
        } else if (it->function == DONT) {
            ++n_dont;
        } else if (it->function == MUL) {
            ++n_mul;
        }
    }

    printf("# do: %lu\n", n_do);
    printf("# dont: %lu\n", n_dont);
    printf("# mul: %lu\n", n_mul);

    int32_t sum = 0;
    bool fn_do = true;
    for (fn_call_t * it = (fn_call_t *)array_t_begin(fn_calls); it != array_t_end(fn_calls); ++it) {
        if (it->function == DO) {
            fn_do = true;
        }

        if (it->function == DONT) {
            fn_do = false;
        }

        if (it->function == MUL && fn_do) {
            sum += it->parameters.mul_params.first * it->parameters.mul_params.second;
        }
    }

    array_t_free(fn_calls);

    return sum;
}