#ifndef IEEE754_HPP
#define IEEE754_HPP
#include <cstdint>
#include <string>

// Класифікація значень згідно зі стандартом IEEE 754
enum class FloatClass {
    zero,
    subnormal,
    normal,
    infinity,
    nan
};

// Структура для збереження розібраних полів binary32 (float)
struct Float32Info {
    std::uint32_t bits;
    bool sign;
    std::uint8_t exponent;
    std::uint32_t fraction;
    FloatClass value_class;
};

// Базові функції безпечного перетворення бітів
std::uint32_t bits_of(float value);
float float_from_bits(std::uint32_t bits);

// Функція аналізу полів
Float32Info analyze_float(float value);
std::string class_to_string(FloatClass fc);

// Функції для демонстрації властивостей та експериментів
void print_type_limits_table();
void run_precision_experiments();
void run_variant1_tests();
void run_variant4_tests();

#endif // IEEE754_HPP