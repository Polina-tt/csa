cmake_minimum_required(VERSION 3.16)
project(operation_order_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(operation_order
    main.cpp
    benchmarks.cpp
)

target_compile_options(operation_order PRIVATE
    $<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
)