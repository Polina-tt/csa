#include "functions.hpp"
#include <stdexcept>

int find_max(const std::vector<int>& values) {
    if (values.empty()) {
        throw std::runtime_error("Vector is empty");
    }
    int maximum = values[0];
    for (std::size_t i = 1; i < values.size(); ++i) {
        if (values[i] > maximum) {
            maximum = values[i];
        }
    }
    return maximum;
}