#include "benchmarks.hpp"

// Спільна частина (розділ 9.1 методички)
std::uint64_t calculate(std::uint64_t x) {
    volatile std::uint64_t unused = x * 100; // невикористане обчислення
    (void)unused;
    
    std::uint64_t constant = 25 * 4; // згортання констант
    std::uint64_t a = x + 2;
    std::uint64_t b = x * 3;
    return a + b + constant;
}

// 1 ланцюжок (залежний)
std::uint64_t add_one(const std::vector<std::uint64_t>& values, std::size_t iterations) {
    std::uint64_t x0 = 1;
    const std::size_t mask = values.size() - 1;
    for (std::size_t i = 0; i < iterations; ++i) {
        x0 += values[i & mask];
    }
    return x0;
}

// 2 ланцюжки (незалежні)
std::uint64_t add_two(const std::vector<std::uint64_t>& values, std::size_t iterations) {
    std::uint64_t x0 = 1, x1 = 2;
    const std::size_t mask = values.size() - 1;
    for (std::size_t i = 0; i < iterations; i += 2) {
        x0 += values[(i + 0) & mask];
        x1 += values[(i + 1) & mask];
    }
    return x0 ^ x1;
}

// 4 ланцюжки
std::uint64_t add_four(const std::vector<std::uint64_t>& values, std::size_t iterations) {
    std::uint64_t x0 = 1, x1 = 2, x2 = 3, x3 = 4;
    const std::size_t mask = values.size() - 1;
    for (std::size_t i = 0; i < iterations; i += 4) {
        x0 += values[(i + 0) & mask];
        x1 += values[(i + 1) & mask];
        x2 += values[(i + 2) & mask];
        x3 += values[(i + 3) & mask];
    }
    return x0 ^ x1 ^ x2 ^ x3;
}

// 8 ланцюжків (самостійна реалізація для варіанта)
std::uint64_t add_eight(const std::vector<std::uint64_t>& values, std::size_t iterations) {
    std::uint64_t x0 = 1, x1 = 2, x2 = 3, x3 = 4;
    std::uint64_t x4 = 5, x5 = 6, x6 = 7, x7 = 8;
    const std::size_t mask = values.size() - 1;
    for (std::size_t i = 0; i < iterations; i += 8) {
        x0 += values[(i + 0) & mask];
        x1 += values[(i + 1) & mask];
        x2 += values[(i + 2) & mask];
        x3 += values[(i + 3) & mask];
        x4 += values[(i + 4) & mask];
        x5 += values[(i + 5) & mask];
        x6 += values[(i + 6) & mask];
        x7 += values[(i + 7) & mask];
    }
    return x0 ^ x1 ^ x2 ^ x3 ^ x4 ^ x5 ^ x6 ^ x7;
}