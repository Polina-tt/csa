#include <iostream>
#include <vector>
#include "functions.hpp"

int main() {
    // Вхідні дані для тестів (можна змінити або зчитувати з консолі)
    std::vector<int> data = {4, 1, 9, 2};
    
    try {
        int result = find_max(data);
        std::cout << "Max element: " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
    return 0;
}