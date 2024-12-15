#pragma once

#include <stdlib.h>

#include "lib/array.h"

size_t parse_i32_numbers(char const * string, array_t * destination);
size_t parse_u64_numbers(char const * string, array_t * destination);