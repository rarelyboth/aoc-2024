cmake_minimum_required(VERSION 3.30)
project(aoc_2024 C)

set(CMAKE_C_STANDARD 23)

add_executable(01 src/01/main.c)
set_target_properties(
    01
    PROPERTIES
    OUTPUT_NAME 01.out
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/01
)
