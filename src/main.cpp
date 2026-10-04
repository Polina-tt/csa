#include "packed_data.hpp"
#include <iostream>
#include <cassert>

void run_mandatory_test() {
    std::cout << "--- Запуск обов'язкового тесту методички ---\n";
    
    // Параметры обязательного теста:
    // заряд 73, режим Active, WiFi true, BT false, ошибка 18, темп +24°C, сервис false
    PackedData device(73, DeviceMode::Active, true, false, 18, 24, false);
    
    std::cout << "Дані успішно запаковані!\n";
    device.print_raw();

    // Проверяем, что все распаковалось в точности, как вводили (Автоматические ассерты)
    assert(device.battery() == 73);
    assert(device.mode() == DeviceMode::Active);
    assert(device.wifi() == true);
    assert(device.bluetooth() == false);
    assert(device.error_code() == 18);
    assert(device.temperature_celsius() == 24);
    assert(device.service_required() == false);
    assert(device.check_reserved_bits() == true);

    std::cout << "Успішно розпаковано та перевірено за допомогою assert!\n\n";

    // Демонстрация изменения одного поля без повреждения других
    std::cout << "Зміна режиму роботи на Charging та перемикання Bluetooth...\n";
    device.set_mode(DeviceMode::Charging);
    device.toggle_bluetooth();

    device.print_raw();
    // Проверяем, что другие поля (заряд и ошибка) НЕ пострадали
    assert(device.battery() == 73);
    assert(device.error_code() == 18);
    assert(device.mode() == DeviceMode::Charging);
    assert(device.bluetooth() == true);
    std::cout << "Перевірка ізольованості полів пройшла успішно!\n\n";
}

void run_negative_tests() {
    std::cout << "--- Запуск негативних тестів (перевірка помилок) ---\n";
    PackedData device;

    // 1. Попытка поставить батарею 105%
    try {
        device.set_battery(105);
        std::cout << "Помилка: Пропущено некоректний заряд батареї!\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Успішно перехоплено помилку батареї: " << e.what() << "\n";
    }

    // 2. Попытка поставить температуру 90 градусов (макс 80)
    try {
        device.set_temperature_celsius(90);
        std::cout << "Помилка: Пропущено некоректну температуру!\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Успішно перехоплено помилку температури: " << e.what() << "\n";
    }
    std::cout << "\n";
}

void compare_sizes() {
    std::cout << "--- Порівняння обсягу пам'яті (sizeof) ---\n";
    std::cout << "Розмір компактного класу PackedData       : " << sizeof(PackedData) << " байт(и)\n";
    std::cout << "Розмір стандартної структури з полями    : " << sizeof(StandardDeviceState) << " байт(и)\n";
    std::cout << "Економіія пам'яті: " 
              << (sizeof(StandardDeviceState) - sizeof(PackedData)) << " байт(ів)!\n";
}

int main() {
    try {
        run_mandatory_test();
        run_negative_tests();
        compare_sizes();
        std::cout << "\nУсі тести та демонстрації виконано на відмінно!\n";
    } catch (const std::exception& e) {
        std::cerr << "Критична помилка виконання програми: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}