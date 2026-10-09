cmake_minimum_required(VERSION 3.16)
project(integer_arithmetic LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(integer_arithmetic
    main.cpp
    arithmetic.cpp
)

target_compile_options(integer_arithmetic PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive->
)