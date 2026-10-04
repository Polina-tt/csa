cmake_minimum_required(VERSION 3.20)
project(lab1_packed_data LANGUAGES CXX)

add_executable(lab1
    src/main.cpp
    src/packed_data.cpp
)

target_include_directories(lab1 PRIVATE include)
target_compile_features(lab1 PRIVATE cxx_std_20)

if (MSVC)
    target_compile_options(lab1 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lab1 PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wsign-conversion
    )
endif()