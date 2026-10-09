#ifndef ARITHMETIC_HPP
#define ARITHMETIC_HPP
#include <cstdint>
#include <string>

// --- 8.1 & 8.2 Допоміжні функції ---
void print_type_properties();
std::string to_binary(std::uint32_t value, unsigned width = 32);

// --- 8.3 Беззнакове переповнення ---
struct UnsignedTestResult {
    std::uint32_t exact_sum;     // Повний точний результат
    std::uint8_t wrapped_result;  // Результат після урізання до 8 біт
    bool carry_or_borrow;        // Ознака перенесення або позики
};
UnsignedTestResult test_unsigned_op_8(std::uint8_t a, std::uint8_t b, char op);

// --- 8.4 Прогноз знакового переповнення ---
struct SignedTestResult {
    std::int16_t exact_sum;      // Обчислення у ширшому типі (16 біт)
    bool fits_in_int8;           // Чи вміщується результат у int8_t (-128..127)
};
SignedTestResult test_signed_add_8(std::int8_t a, std::int8_t b);

// --- 9.1 ВАРІАНТ 1: Додавання та віднімання з контролем стану ---
struct Add8Result {
    std::uint8_t wrapped;        // Циклічний результат (8 біт)
    bool carry;                  // Беззнакове перенесення
    bool signed_overflow;        // Знакове переповнення
};

struct Sub8Result {
    std::uint8_t wrapped;
    bool borrow;                 // Беззнакова позика
    bool signed_overflow;
};

Add8Result add8(std::uint8_t a, std::uint8_t b);
Sub8Result sub8(std::uint8_t a, std::uint8_t b);

#endif // ARITHMETIC_HPP