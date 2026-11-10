#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <cstdint>
#include "benchmarks.hpp"

struct Measurement {
    double milliseconds;
    std::uint64_t checksum;
};

template<class Function>
Measurement measure_once(Function function) {
    const auto start = std::chrono::steady_clock::now();
    const std::uint64_t checksum = function();
    const auto finish = std::chrono::steady_clock::now();
    const double ms = std::chrono::duration<double, std::milli>(finish - start).count();
    return {ms, checksum};
}

double median(std::vector<double> values) {
    std::sort(values.begin(), values.end());
    const std::size_t middle = values.size() / 2;
    if (values.size() % 2 == 1) {
        return values[middle];
    }
    return (values[middle - 1] + values[middle]) / 2.0;
}

template<class Function>
Measurement measure_series(Function function, int repeats = 7) {
    function(); // прогрів
    std::vector<double> times;
    std::uint64_t checksum = 0;
    for (int repeat = 0; repeat < repeats; ++repeat) {
        const Measurement current = measure_once(function);
        times.push_back(current.milliseconds);
        checksum ^= current.checksum;
    }
    return {median(times), checksum};
}

int main() {
    // Перевірка спільної частини
    std::cout << "Calculate(7) = " << calculate(7) << "\n\n";

    const std::size_t data_size = 1024;
    std::vector<std::uint64_t> data(data_size, 1);
    const std::size_t iterations = 200'000'000;

    auto run_1 = [&]() { return add_one(data, iterations); };
    auto run_2 = [&]() { return add_two(data, iterations); };
    auto run_4 = [&]() { return add_four(data, iterations); };
    auto run_8 = [&]() { return add_eight(data, iterations); };

    std::cout << "Running measurements...\n";

    auto m1 = measure_series(run_1);
    auto m2 = measure_series(run_2);
    auto m4 = measure_series(run_4);
    auto m8 = measure_series(run_8);

    std::cout << "\nLanes\tMedian (ms)\tUpdates/s\tChecksum\n";
    std::cout << "-------------------------------------------------\n";
    std::cout << "1\t" << m1.milliseconds << "\t\t" << (iterations * 1000.0 / m1.milliseconds) << "\t" << m1.checksum << "\n";
    std::cout << "2\t" << m2.milliseconds << "\t\t" << (iterations * 1000.0 / m2.milliseconds) << "\t" << m2.checksum << "\n";
    std::cout << "4\t" << m4.milliseconds << "\t\t" << (iterations * 1000.0 / m4.milliseconds) << "\t" << m4.checksum << "\n";
    std::cout << "8\t" << m8.milliseconds << "\t\t" << (iterations * 1000.0 / m8.milliseconds) << "\t" << m8.checksum << "\n";

    return 0;
}