#include <iostream>
#include <iomanip>
#include "arithmetic.hpp"

void run_section_8() {
    std::cout << "==================================================\n";
    std::cout << "   СПІЛЬНА ЧАСТИНА ДЛЯ ВСІХ ВАРІАНТІВ (РОЗДІЛ 8)  \n";
    std::cout << "==================================================\n\n";

    std::cout << "[8.1. Властивості типів даних]\n";
    print_type_properties();
    std::cout << "\n";

    std::cout << "[8.2. Двійкове та HEX подання числа 0xA5B2C3D4]\n";
    std::uint32_t test_val = 0xA5B2C3D4;
    std::cout << "DEC: " << test_val << "\n";
    std::cout << "HEX: 0x" << std::hex << std::uppercase << test_val << std::dec << "\n";
    std::cout << "BIN: " << to_binary(test_val, 32) << "\n\n";

    std::cout << "[8.3. Дослідження беззнакового переповнення (uint8_t)]\n";
    auto t1 = test_unsigned_op_8(250, 10, '+');
    std::cout << "250 + 10  -> Точно: " << t1.exact_sum << " | 8-біт: " << +t1.wrapped_result << " | Carry: " << t1.carry_or_borrow << "\n";
    
    auto t2 = test_unsigned_op_8(0, 1, '-');
    std::cout << "0 - 1     -> Точно: " << t2.exact_sum << " | 8-біт: " << +t2.wrapped_result << " | Borrow: " << t2.carry_or_borrow << "\n\n";

    std::cout << "[8.4. Прогноз знакового переповнення (int8_t)]\n";
    auto s1 = test_signed_add_8(120, 20);
    std::cout << "120 + 20   -> Точно: " << s1.exact_sum << " | Вміщується в int8_t: " << (s1.fits_in_int8 ? "Так" : "НІ (Переповнення!)") << "\n";

    auto s2 = test_signed_add_8(-100, 30);
    std::cout << "-100 + 30  -> Точно: " << s2.exact_sum << " | Вміщується в int8_t: " << (s2.fits_in_int8 ? "Так" : "НІ") << "\n\n";
}

void run_variant_1_tests() {
    std::cout << "==================================================\n";
    std::cout << "   ОБОВ'ЯЗКОВІ ТЕСТИ ВАРІАНТА 1 (РОЗДІЛ 9.1)       \n";
    std::cout << "==================================================\n\n";

    auto print_add = [](int test_num, std::uint8_t a, std::uint8_t b) {
        auto r = add8(a, b);
        std::cout << "Тест " << test_num << " (+): a=" << +a << " (0x" << std::hex << +a << std::dec 
                  << "), b=" << +b << " (0x" << std::hex << +b << std::dec << ")\n"
                  << "   -> Wrapped: " << +r.wrapped << " (0x" << std::hex << +r.wrapped << std::dec << ")\n"
                  << "   -> Carry: " << r.carry << " | Signed Overflow: " << r.signed_overflow << "\n\n";
    };

    auto print_sub = [](int test_num, std::uint8_t a, std::uint8_t b) {
        auto r = sub8(a, b);
        std::cout << "Тест " << test_num << " (-): a=" << +a << " (0x" << std::hex << +a << std::dec 
                  << "), b=" << +b << " (0x" << std::hex << +b << std::dec << ")\n"
                  << "   -> Wrapped: " << +r.wrapped << " (0x" << std::hex << +r.wrapped << std::dec << ")\n"
                  << "   -> Borrow: " << r.borrow << " | Signed Overflow: " << r.signed_overflow << "\n\n";
    };

    print_add(1, 10, 20);        // Звичайне додавання
    print_add(2, 250, 10);       // Беззнакове перенесення
    print_add(3, 0x78, 0x14);    // 120 + 20: Знакове переповнення (два додатні)
    print_add(4, 0x9C, 0xEC);    // -100 + (-20): Два від'ємні
    print_sub(5, 3, 5);          // Беззнакова позика
    print_sub(6, 0x80, 0x01);    // -128 - 1: Вихід нижче -128
}

int main() {
    run_section_8();
    run_variant_1_tests();
    return 0;
}