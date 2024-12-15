cmake_minimum_required(VERSION 3.30)
project(aoc_2024 C)

set(CMAKE_C_STANDARD 23)

add_subdirectory(lib)

function(add_day day sources)
    add_executable(${day} ${sources})
    set_target_properties(
        ${day}
        PROPERTIES
        OUTPUT_NAME ${day}.out
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/${day}
    )
    target_include_directories(${day} PRIVATE lib/include)
    target_link_libraries(${day} lib)
endfunction()

add_day(01 src/01/main.c)
add_day(02 src/02/main.c)
add_day(03 src/03/main.c)
add_day(04 src/04/main.c)
add_day(05 src/05/main.c)
add_day(06 src/06/main.c)
add_day(07 src/07/main.c)
add_day(08 src/08/main.c)
add_day(09 src/09/main.c)