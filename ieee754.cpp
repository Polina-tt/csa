#include "ieee754.hpp"
#include <iostream>
#include <bit>
#include <limits>
#include <iomanip>
#include <cmath>

// 4.6. Безпечне отримання бітового шаблону через std::bit_cast (C++20)
std::uint32_t bits_of(float value) {
    return std::bit_cast<std::uint32_t>(value);
}

float float_from_bits(std::uint32_t bits) {
    return std::bit_cast<float>(bits);
}

// 8.2. Аналізатор binary32: виділення полів S, E, F за допомогою масок та зсувів
Float32Info analyze_float(float value) {
    std::uint32_t bits = bits_of(value);
    
    bool sign = (bits >> 31) & 0x01;
    std::uint8_t exponent = static_cast<std::uint8_t>((bits >> 23) & 0xFFu);
    std::uint32_t fraction = bits & 0x7FFFFFu;
    
    FloatClass fc;
    // 4.4. Класифікація за значенням E та F
    if (exponent == 0) {
        fc = (fraction == 0) ? FloatClass::zero : FloatClass::subnormal;
    } else if (exponent == 255) {
        fc = (fraction == 0) ? FloatClass::infinity : FloatClass::nan;
    } else {
        fc = FloatClass::normal;
    }
    
    return {bits, sign, exponent, fraction, fc};
}

std::string class_to_string(FloatClass fc) {
    switch (fc) {
        case FloatClass::zero: return "zero";
        case FloatClass::subnormal: return "subnormal";
        case FloatClass::normal: return "normal";
        case FloatClass::infinity: return "infinity";
        case FloatClass::nan: return "nan";
    }
    return "unknown";
}

// 8.1. Виведення властивостей типів через std::numeric_limits
void print_type_limits_table() {
    std::cout << "=== 8.1. Властивості типів даних ===\n";
    std::cout << std::left << std::setw(12) << "Тип"
              << std::setw(8) << "sizeof"
              << std::setw(12) << "is_iec559"
              << std::setw(8) << "digits"
              << std::setw(10) << "digits10"
              << std::setw(12) << "max_digits10"
              << std::setw(18) << "epsilon" << "\n";
    std::cout << std::string(75, '-') << "\n";

    auto print_row = [](const auto& name, auto t) {
        using T = std::decay_t<decltype(t)>;
        std::cout << std::left << std::setw(12) << name
                  << std::setw(8) << sizeof(T)
                  << std::setw(12) << (std::numeric_limits<T>::is_iec559 ? "true" : "false")
                  << std::setw(8) << std::numeric_limits<T>::digits
                  << std::setw(10) << std::numeric_limits<T>::digits10
                  << std::setw(12) << std::numeric_limits<T>::max_digits10
                  << std::scientific << std::setprecision(3) << std::numeric_limits<T>::epsilon() << "\n";
    };

    print_row("float", 0.0f);
    print_row("double", 0.0);
    print_row("long double", 0.0l);
    std::cout << "\n";
}

// 8.4. Досліди з точністю (0.1 + 0.2, втрата точності великих чисел, неасоціативність)
void run_precision_experiments() {
    std::cout << "=== 8.4. Досліди з точністю ===\n";

    // 1. Додавання 0.1 + 0.2
    float f1 = 0.1f + 0.2f;
    double d1 = 0.1 + 0.2;
    std::cout << "1. 0.1 + 0.2:\n";
    std::cout << "   float  (max_digits10): " << std::setprecision(std::numeric_limits<float>::max_digits10) << f1 << "\n";
    std::cout << "   double (max_digits10): " << std::setprecision(std::numeric_limits<double>::max_digits10) << d1 << "\n";

    // 4. Додавання малого до великого (10^8 + 1.0)
    float large = 100000000.0f;
    float added = large + 1.0f;
    std::cout << "4. Додавання 1.0 до 10^8 у float: велике = " << std::fixed << large 
              << ", результат = " << added << " (зміни немає через брак бітів мантиси)\n";

    // 5. Неасоціативність: (a + b) + c проти a + (b + c)
    float a = 1e20f, b = -1e20f, c = 3.0f;
    std::cout << "5. Неасоціативність (a=1e20, b=-1e20, c=3):\n";
    std::cout << "   (a + b) + c = " << (a + b) + c << "\n";
    std::cout << "   a + (b + c) = " << a + (b + c) << "\n\n";
}

// Варіант 1: Конструктор float із бітових полів
struct BuildResult {
    float value;
    bool valid;
};

BuildResult build_float(bool sign, std::uint16_t exponent, std::uint32_t fraction) {
    if (exponent > 255 || fraction > 0x7FFFFFu) {
        return { 0.0f, false };
    }
    std::uint32_t s_bit = (sign ? 1u : 0u) << 31;
    std::uint32_t e_bits = (static_cast<std::uint32_t>(exponent) & 0xFFu) << 23;
    std::uint32_t f_bits = fraction & 0x7FFFFFu;

    std::uint32_t raw_bits = s_bit | e_bits | f_bits;
    return { std::bit_cast<float>(raw_bits), true };
}

void run_variant1_tests() {
    std::cout << "=== Варіант 1. Конструктор і декодер binary32 ===\n";
    auto res1 = build_float(0, 127, 0x000000); // 1.0f
    auto res2 = build_float(1, 128, 0x200000); // -2.5f
    std::cout << "Зібрано з S=0, E=127, F=0 -> Значення: " << res1.value << "\n";
    std::cout << "Зібрано з S=1, E=128, F=0x200000 -> Значення: " << res2.value << "\n\n";
}