#include "arithmetic.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <bitset>

// 8.1. Виведення меж типів
void print_type_properties() {
    std::cout << std::left 
              << std::setw(15) << "Тип" 
              << std::setw(15) << "sizeof (байт)" 
              << std::setw(15) << "Біти" 
              << std::setw(20) << "Мінімум" 
              << "Максимум\n";
    std::cout << std::string(80, '-') << "\n";

    auto print_row = [](const char* name, auto min_val, auto max_val, size_t size) {
        // Явне (+min_val) або static_cast<int> потрібні, щоб std::uint8_t 
        // виводився як ЧИСЛО, а не як ASCII-символ!
        std::cout << std::setw(15) << name 
                  << std::setw(15) << size 
                  << std::setw(15) << size * 8 
                  << std::setw(20) << +min_val 
                  << +max_val << "\n";
    };

    print_row("std::int8_t",   std::numeric_limits<std::int8_t>::min(),   std::numeric_limits<std::int8_t>::max(),   sizeof(std::int8_t));
    print_row("std::uint8_t",  std::numeric_limits<std::uint8_t>::min(),  std::numeric_limits<std::uint8_t>::max(),  sizeof(std::uint8_t));
    print_row("std::int16_t",  std::numeric_limits<std::int16_t>::min(),  std::numeric_limits<std::int16_t>::max(),  sizeof(std::int16_t));
    print_row("std::uint16_t", std::numeric_limits<std::uint16_t>::min(), std::numeric_limits<std::uint16_t>::max(), sizeof(std::uint16_t));
    print_row("std::int32_t",  std::numeric_limits<std::int32_t>::min(),  std::numeric_limits<std::int32_t>::max(),  sizeof(std::int32_t));
    print_row("std::uint32_t", std::numeric_limits<std::uint32_t>::min(), std::numeric_limits<std::uint32_t>::max(), sizeof(std::uint32_t));
}

// 8.2. Двійкове подання з групуванням по 4 біти
std::string to_binary(std::uint32_t value, unsigned width) {
    std::string full_bin = std::bitset<32>(value).to_string();
    std::string cropped = full_bin.substr(32 - width);
    std::string grouped;
    for (size_t i = 0; i < cropped.size(); ++i) {
        if (i > 0 && (cropped.size() - i) % 4 == 0) grouped += ' ';
        grouped += cropped[i];
    }
    return grouped;
}

// 8.3. Дослід із беззнаковим переповненням
UnsignedTestResult test_unsigned_op_8(std::uint8_t a, std::uint8_t b, char op) {
    UnsignedTestResult res{};
    if (op == '+') {
        // Перетворюємо до uint32_t, щоб сума НЕ втратила біти
        res.exact_sum = static_cast<std::uint32_t>(a) + b;
        // Кастуємо назад до uint8_t — тут C++ робить циклічний перехід (wrap-around)
        res.wrapped_result = static_cast<std::uint8_t>(a + b);
        // Якщо точний результат > 255, виникло беззнакове перенесення (Carry)
        res.carry_or_borrow = (res.exact_sum > 255);
    } else if (op == '-') {
        std::int32_t diff = static_cast<std::int32_t>(a) - b;
        res.exact_sum = static_cast<std::uint32_t>(diff);
        res.wrapped_result = static_cast<std::uint8_t>(a - b);
        // Якщо a < b, то для віднімання потрібна була позика (Borrow)
        res.carry_or_borrow = (a < b);
    }
    return res;
}

// 8.4. Прогноз знакового переповнення
SignedTestResult test_signed_add_8(std::int8_t a, std::int8_t b) {
    // Спочатку кастуємо до int16_t і додаємо у ширшому типі.
    // Це ГАРАНТУЄ відсутність Невизначеної Поведінки (UB)!
    std::int16_t exact = static_cast<std::int16_t>(a) + static_cast<std::int16_t>(b);
    
    // Перевіряємо, чи входить результат у межі int8_t [-128...127]
    bool fits = (exact >= std::numeric_limits<std::int8_t>::min() && 
                 exact <= std::numeric_limits<std::int8_t>::max());
    return {exact, fits};
}

// 9.1. ВАРІАНТ 1: Додавання 8-біт з подвійною інтерпретацією
Add8Result add8(std::uint8_t a, std::uint8_t b) {
    // 1. Беззнакова інтерпретація
    std::uint32_t exact_u = static_cast<std::uint32_t>(a) + b;
    bool carry = (exact_u > 255);

    // 2. Знакова інтерпретація (ті самі біти трактуємо як int8_t)
    auto sa = static_cast<std::int8_t>(a);
    auto sb = static_cast<std::int8_t>(b);
    std::int16_t exact_s = static_cast<std::int16_t>(sa) + static_cast<std::int16_t>(sb);
    
    // Знакове переповнення перевіряємо за математичним результатом у int16_t
    bool s_overflow = (exact_s < -128 || exact_s > 127);

    return { static_cast<std::uint8_t>(a + b), carry, s_overflow };
}

// 9.1. ВАРІАНТ 1: Віднімання 8-біт
Sub8Result sub8(std::uint8_t a, std::uint8_t b) {
    bool borrow = (a < b);

    auto sa = static_cast<std::int8_t>(a);
    auto sb = static_cast<std::int8_t>(b);
    std::int16_t exact_s = static_cast<std::int16_t>(sa) - static_cast<std::int16_t>(sb);
    bool s_overflow = (exact_s < -128 || exact_s > 127);

    return { static_cast<std::uint8_t>(a - b), borrow, s_overflow };
}