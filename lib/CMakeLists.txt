cmake_minimum_required(VERSION 3.30)
project(lib C)

set(CMAKE_C_STANDARD 23)

include_directories(include)

set(include
    include/lib/array.h
    include/lib/file.h
    include/lib/parse.h
)

set(src
    src/array.c
    src/file.c
    src/parse.c
)

add_library(lib STATIC ${includes} ${src})