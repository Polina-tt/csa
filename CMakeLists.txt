cmake_minimum_required(VERSION 3.16)
project(ieee754_lab LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Збираємо виконавчий файл із трьох модулів
add_executable(ieee754_lab main.cpp ieee754.cpp ieee754.hpp)

# Увімкнення обов'язкових попереджень компілятора
target_compile_options(ieee754_lab PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
)