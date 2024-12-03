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
