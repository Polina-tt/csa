#pragma once

#include <vector>
#include <cstdint>
#include <cstddef>

// Спільна частина
std::uint64_t calculate(std::uint64_t x);

// Варіант 1: акумулятори (1, 2, 4, 8)
std::uint64_t add_one(const std::vector<std::uint64_t>& values, std::size_t iterations);
std::uint64_t add_two(const std::vector<std::uint64_t>& values, std::size_t iterations);
std::uint64_t add_four(const std::vector<std::uint64_t>& values, std::size_t iterations);
std::uint64_t add_eight(const std::vector<std::uint64_t>& values, std::size_t iterations);