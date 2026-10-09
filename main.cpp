#include "ieee754.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <bitset>

void test_mandatory_templates() {
    std::cout << "=== 8.3. Перевірка обов’язкових бітових шаблонів ===\n";
    
    float test_vals[] = { +0.0f, -0.0f, 1.0f, -2.5f, 1.0f / 0.0f };
    for (float v : test_vals) {
        auto info = analyze_float(v);
        std::cout << std::setw(10) << v 
                  << " | Клас: " << std::setw(10) << class_to_string(info.value_class)
                  << " | Hex: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << info.bits 
                  << std::dec << std::setfill(' ') << "\n";
    }
    
    // Перевірка NaN
    float nan_val = std::numeric_limits<float>::quiet_NaN();
    auto nan_info = analyze_float(nan_val);
    std::cout << std::setw(10) << "NaN"
              << " | Клас: " << std::setw(10) << class_to_string(nan_info.value_class)
              << " | Hex: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << nan_info.bits 
              << std::dec << std::setfill(' ') << "\n\n";
}

int main() {
    std::cout << "Лабораторна робота №3: Подання чисел у форматі IEEE 754\n\n";

    // 1. Виведення таблиці лімітів типів (спільна частина 8.1)
    print_type_limits_table();

    // 2. Перевірка бітових шаблонів (спільна частина 8.3)
    test_mandatory_templates();

    // 3. Експерименти з точністю (спільна частина 8.4)
    run_precision_experiments();

    // 4. Виконання індивідуального варіанта (Варіант 1)
    run_variant1_tests();

    return 0;
}