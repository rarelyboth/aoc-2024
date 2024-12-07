#include "lib/file.h"

FILE * file_open(char const * filename, char const * modes) {
    FILE * file = fopen(filename, modes);

    if (file == nullptr) {
        fprintf(stderr, "Failed to open file '%s' in '%s' modes.", filename, modes);
    }

    return file;
}
